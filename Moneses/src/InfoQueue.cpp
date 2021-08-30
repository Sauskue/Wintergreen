#include "pch.h"
#include "InfoQueue.h"

static IDXGIInfoQueue* dxgi_info_queue;
static bool initialized = false;

void InfoQueue::Init()
{
    if (initialized)
        return;
    typedef HRESULT(WINAPI* DXGIGDIFP)(REFIID, void**);
    HMODULE dxgi_debug_dll = LoadLibraryEx(L"dxgidebug.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (dxgi_debug_dll)
    {
        auto DXGIGetDebugDevice = (DXGIGDIFP)GetProcAddress(dxgi_debug_dll, "DXGIGetDebugInterface");
        if (DXGIGetDebugDevice)
        {
            if (SUCCEEDED(DXGIGetDebugDevice(__uuidof(IDXGIInfoQueue), (void**)&dxgi_info_queue)))
            {
                dxgi_info_queue->ClearStoredMessages(DXGI_DEBUG_ALL);
                dxgi_info_queue->PushEmptyRetrievalFilter(DXGI_DEBUG_ALL);
                dxgi_info_queue->PushEmptyStorageFilter(DXGI_DEBUG_ALL);
                initialized = true;
                LOG("DXGIInfoQueue initialized");
                return;
            }
            else
                LOG("Could not retrieve debugging interface", LogLevel::Error);
        }
        else
            LOG("Could not load DXGIGetDebugInterface function", LogLevel::Error);
    }
    else
    {
        LOG("Could not load DXGIDebug.dll", LogLevel::Error);
    }
    LOG("Could not initialize DXGIInfoQueue", LogLevel::Error);
}

void InfoQueue::ProcessMessages()
{
    if (!initialized)
    {
        LOG("DXGIInfoQueue has not been initialized yet", LogLevel::Error);
        return;
    }
    for (int i = 0; i < dxgi_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL); i++)
    {
        DXGI_INFO_QUEUE_MESSAGE* msg;
        SIZE_T msg_length = 0;
        dxgi_info_queue->GetMessage(DXGI_DEBUG_ALL, i, NULL, &msg_length);
        msg = (DXGI_INFO_QUEUE_MESSAGE*)malloc(msg_length);
        dxgi_info_queue->GetMessage(DXGI_DEBUG_ALL, i, msg, &msg_length);
        if (msg != nullptr)
        {
            LogLevel severity = LogLevel::None;
            switch (msg->Severity)
            {
                case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:
                {
                    severity = LogLevel::Error;
                    break;
                }

                case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
                {
                    severity = LogLevel::Error;
                    break;
                }

                case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
                {
                    severity = LogLevel::Warning;
                    break;
                }

                case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:
                {
                    severity = LogLevel::Info;
                    break;
                }

                case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:
                {
                    severity = LogLevel::Debug;
                    break;
                }
            }
            LOG(msg->pDescription, severity);
            free(msg);
        }
        else
            LOG("InfoQueue message could not be processed!", LogLevel::Error);
    }
    dxgi_info_queue->ClearStoredMessages(DXGI_DEBUG_ALL);
}

void InfoQueue::Cleanup()
{
    //Is this all that needs to happen???
    if (dxgi_info_queue)
        dxgi_info_queue->Release();
}