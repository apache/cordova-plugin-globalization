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
#include <json/reader.h>
#include <json/writer.h>
#include "globalization_ndk.hpp"
#include "globalization_js.hpp"

/*
 * The following constants are defined based on Cordova Globalization
 * plugin definition. They should match exactly.
 * https://github.com/apache/cordova-plugin-globalization/blob/master/doc/index.md
*/
const int UNKNOWN_ERROR = 0;
const int FORMATTING_ERROR = 1;
const int PARSING_ERROR = 2;
const int PATTERN_ERROR = 3;

namespace webworks {

std::string errorInJson(int code, const std::string& message)
{
    Json::Value error;
    error["code"] = code;
    error["message"] = message;

    Json::Value root;
    root["error"] = error;

    Json::FastWriter writer;
    return writer.write(root);
}

GlobalizationNDK::GlobalizationNDK(GlobalizationJS *parent) {
	m_pParent = parent;
}

GlobalizationNDK::~GlobalizationNDK() {
}

std::string GlobalizationNDK::getPreferredLanguage()
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::getLocaleName()
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::dateToString(const std::string& args)
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::stringToDate(const std::string& args)
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::getDatePattern(const std::string& args)
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::getDateNames(const std::string& args)
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::isDayLightSavingsTime(const std::string& args)
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::getFirstDayOfWeek()
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::numberToString(const std::string& args)
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::stringToNumber(const std::string& args)
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::getNumberPattern(const std::string& args)
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

std::string GlobalizationNDK::getCurrencyPattern(const std::string& args)
{
    return errorInJson(UNKNOWN_ERROR, "Not supported!");
}

} /* namespace webworks */
