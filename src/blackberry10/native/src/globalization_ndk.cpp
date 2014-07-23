/*
 * Copyright 2013 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include <sstream>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "globalization_ndk.hpp"
#include "globalization_js.hpp"

namespace webworks {

GlobalizationNDK::GlobalizationNDK(GlobalizationJS *parent) {
	m_pParent = parent;
	globalizationProperty = 50;
	globalizationThreadCount = 1;
	m_thread = 0;
	pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	threadHalt = true;
}

GlobalizationNDK::~GlobalizationNDK() {
}

// These methods are the true native code we intend to reach from WebWorks
std::string GlobalizationNDK::globalizationTestString() {
	return "Globalization Test Function";
}

// Take in input and return a value
std::string GlobalizationNDK::globalizationTestString(const std::string& inputString) {
	return "Globalization Test Function, got: " + inputString;
}

// Get an integer property
std::string GlobalizationNDK::getGlobalizationProperty() {
	stringstream ss;
	ss << globalizationProperty;
	return ss.str();
}

// set an integer property
void GlobalizationNDK::setGlobalizationProperty(const std::string& inputString) {
	globalizationProperty = (int) strtoul(inputString.c_str(), NULL, 10);
}

// Asynchronous callback with JSON data input and output
void GlobalizationNDK::globalizationTestAsync(const std::string& callbackId, const std::string& inputString) {
	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	bool parse = reader.parse(inputString, root);

	if (!parse) {
		Json::Value error;
		error["result"] = "Cannot parse JSON object";
		m_pParent->NotifyEvent(callbackId + " " + writer.write(error));
	} else {
		root["result"] = root["value1"].asInt() + root["value2"].asInt();
		m_pParent->NotifyEvent(callbackId + " " + writer.write(root));
	}
}

// Thread functions
// The following functions are for controlling a Thread in the extension

// The actual thread (must appear before the startThread method)
// Loops and runs the callback method
void* GlobalizationThread(void* parent) {
	GlobalizationNDK *pParent = static_cast<GlobalizationNDK *>(parent);

	// Loop calls the callback function and continues until stop is set
	while (!pParent->isThreadHalt()) {
		sleep(1);
		pParent->globalizationThreadCallback();
	}

	return NULL;
}

// Starts the thread and returns a message on status
std::string GlobalizationNDK::globalizationStartThread(const std::string& callbackId) {
	if (!m_thread) {
		int rc;
	    rc = pthread_mutex_lock(&mutex);
	    threadHalt = false;
	    rc = pthread_cond_signal(&cond);
	    rc = pthread_mutex_unlock(&mutex);

		pthread_attr_t thread_attr;
		pthread_attr_init(&thread_attr);
		pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

		pthread_create(&m_thread, &thread_attr, GlobalizationThread,
				static_cast<void *>(this));
		pthread_attr_destroy(&thread_attr);
		threadCallbackId = callbackId;
		return "Thread Started";
	} else {
		return "Thread Running";
	}
}

// Sets the stop value
std::string GlobalizationNDK::globalizationStopThread() {
	int rc;
	// Request thread to set prevent sleep to false and terminate
	rc = pthread_mutex_lock(&mutex);
	threadHalt = true;
	rc = pthread_cond_signal(&cond);
	rc = pthread_mutex_unlock(&mutex);

    // Wait for the thread to terminate.
    void *exit_status;
    rc = pthread_join(m_thread, &exit_status) ;

	// Clean conditional variable and mutex
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);

	m_thread = 0;
	threadHalt = true;
	return "Thread stopped";
}

// The callback method that sends an event through JNEXT
void GlobalizationNDK::globalizationThreadCallback() {
	Json::FastWriter writer;
	Json::Value root;
	root["threadCount"] = globalizationThreadCount++;
	m_pParent->NotifyEvent(threadCallbackId + " " + writer.write(root));
}

// getter for the stop value
bool GlobalizationNDK::isThreadHalt() {
	int rc;
	bool isThreadHalt;
	rc = pthread_mutex_lock(&mutex);
	isThreadHalt = threadHalt;
	rc = pthread_mutex_unlock(&mutex);
	return isThreadHalt;
}

} /* namespace webworks */
