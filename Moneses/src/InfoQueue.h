#pragma once
class InfoQueue
{
public:
	static void Init();
	static void ProcessMessages();
	static void Cleanup();

	InfoQueue() = delete;
	InfoQueue(const InfoQueue& other) = delete;
	const InfoQueue& operator=(const InfoQueue& other) = delete;
};