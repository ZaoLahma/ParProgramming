/*
 * wordfinder.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: janne
 */


#include "wordfinder.h"

#include <iostream>

std::mutex WordFinder::instanceMutex;

WordFinder::WordFinder(const std::string& string, const std::string& wordToFind, const uint32_t _instanceNo) :
jobData(string, wordToFind),
instanceNo(_instanceNo)
{
	JobDispatcher::GetApi()->SubscribeToEvent(WORD_FINDER_JOB_FINISHED, this);

	WordFinderJob* jobPtr = new WordFinderJob(instanceNo);

	jobPtr->SetJobData(&jobData);

	JobDispatcher::GetApi()->ExecuteJob(jobPtr);
}

void WordFinder::HandleEvent(const uint32_t eventNo, const EventDataBase* dataPtr)
{
	const WordFinderJobFinishedEventData* eventDataPtr = static_cast<const WordFinderJobFinishedEventData*>(dataPtr);

	if(eventDataPtr->instanceId == instanceNo)
	{
		JobDispatcher::GetApi()->UnsubscribeToEvent(WORD_FINDER_JOB_FINISHED, this);
		std::lock_guard<std::mutex> printLock(instanceMutex);
		std::cout<<"String: "<<jobData.wordString<<". Word: "<<jobData.wordToFind<<". No of occurances: "<<jobData.noOfOccurances<<std::endl;
	}
}
