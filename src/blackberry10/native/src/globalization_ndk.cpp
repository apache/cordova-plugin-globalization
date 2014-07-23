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
#include <sys/slog2.h>
#include <unicode/calendar.h>
#include <unicode/datefmt.h>
#include <unicode/dtfmtsym.h>
#include <unicode/smpdtfmt.h>
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

std::string resultInJson(const std::string& value)
{
    Json::Value root;
    root["result"] = value;

    Json::FastWriter writer;
    return writer.write(root);
}

std::string resultInJson(const UDate& date)
{
    UErrorCode status = U_ZERO_ERROR;
    Calendar* cal = Calendar::createInstance(status);
    if (!cal) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::resultInJson: failed to create Calendar instance: %d",
                status);
        return errorInJson(UNKNOWN_ERROR, "Failed to create Calendar instance!");
    }
    cal->setTime(date, status);
    if (status != U_ZERO_ERROR && status != U_ERROR_WARNING_START) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::resultInJson: failed to setTime: %d",
                status);
        delete cal;
        return errorInJson(UNKNOWN_ERROR, "Failed to set Calendar time!");
    }

    Json::Value result;
    result["year"] = cal->get(UCAL_YEAR, status);
    result["month"] = cal->get(UCAL_MONTH, status);
    result["day"] = cal->get(UCAL_DAY_OF_MONTH, status);
    result["hour"] = cal->get(UCAL_HOUR, status);
    result["minute"] = cal->get(UCAL_MINUTE, status);
    result["second"] = cal->get(UCAL_SECOND, status);
    result["millisecond"] = cal->get(UCAL_MILLISECOND, status);

    delete cal;

    Json::Value root;
    root["result"] = result;

    Json::FastWriter writer;
    return writer.write(root);
}

std::string resultInJson(const std::string& pattern, const std::string& timezone, int utc_offset, int dst_offset)
{
    Json::Value result;
    result["pattern"] = pattern;
    result["timezone"] = timezone;
    result["utc_offset"] = utc_offset;
    result["dst_offset"] = dst_offset;

    Json::Value root;
    root["result"] = result;

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
    const Locale& loc = Locale::getDefault();

    UnicodeString disp;
    loc.getDisplayLanguage(loc, disp);
    if (disp.isEmpty())
        return resultInJson("English"); // FIXME: what should be the default language?

    std::string utf8;
    disp.toUTF8String(utf8);
    return resultInJson(utf8);
}

std::string GlobalizationNDK::getLocaleName()
{
    const Locale& loc = Locale::getDefault();
    const char* name = loc.getName();
    if (name)
        return resultInJson(name);

    const char* lang = loc.getLanguage();
    if (!lang)
        return resultInJson("en"); // FIXME: what should be the default language?

    const char* country = loc.getCountry();
    if (!country)
        return resultInJson(lang);

    return resultInJson(std::string(lang) + "_" + country);
}

static bool handleDateOptions(const Json::Value& options, DateFormat::EStyle& dateStyle, DateFormat::EStyle& timeStyle, std::string& error)
{
    // This is the default value when no options provided.
    dateStyle = DateFormat::kShort;
    timeStyle = DateFormat::kShort;

    if (options.isNull())
        return true;

    if (!options.isObject()) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleDateOptions: invalid options format: %d",
                options.type());
        error = "Options is invalid!";
        return false;
    }

    Json::Value flv = options["formatLength"];
    if (!flv.isNull()) {
        if (!flv.isString()) {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleDateOptions: invalid formatLength format: %d",
                    flv.type());
            error = "formatLength is invalid!";
            return false;
        }

        std::string format = flv.asString();
        if (format.empty()) {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleDateOptions: empty formatLength!");
            error = "formatLength is empty!";
            return false;
        }

        if (format == "full") {
            dateStyle = DateFormat::kFull;
            timeStyle = dateStyle;
        } else if (format == "long") {
            dateStyle = DateFormat::kLong;
            timeStyle = dateStyle;
        } else if (format == "medium") {
            dateStyle = DateFormat::kMedium;
            timeStyle = dateStyle;
        } else if (format == "short") {
            // Nothing to change here.
        } else {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleDateOptions: unsupported formatLength: %s",
                    format.c_str());
            error = "Unsupported formatLength!";
            return false;
        }
    }

    Json::Value slv = options["selector"];
    if (!slv.isNull()) {
        if (!slv.isString()) {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleDateOptions: invalid selector format: %d",
                    slv.type());
            error = "selector is invalid!";
            return false;
        }

        std::string selector = slv.asString();
        if (selector.empty()) {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleDateOptions: empty selector!");
            error = "selector is empty!";
            return false;
        }

        if (selector == "date")
            timeStyle = DateFormat::kNone;
            // Nothing to change here
        else if (selector == "time")
            dateStyle = DateFormat::kNone;
        else if (selector == "date and time") {
            // Nothing to do here.
        } else {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleDateOptions: unsupported selector: %s",
                    selector.c_str());
            error = "Unsupported selector!";
            return false;
        }
    }

    return true;
}

std::string GlobalizationNDK::dateToString(const std::string& args)
{
    if (args.empty())
        return errorInJson(PARSING_ERROR, "No date provided!");

    Json::Reader reader;
    Json::Value root;
    bool parse = reader.parse(args, root);

    if (!parse) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::dateToString: invalid json data: %s",
                args.c_str());
        return errorInJson(PARSING_ERROR, "Parameters not valid json format!");
    }

    Json::Value date = root["date"];
    if (date.isNull()) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::dateToString: no date provided.");
        return errorInJson(PARSING_ERROR, "No date provided!");
    }

    if (!date.isDouble()) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::dateToString: date is not a Double: %d.",
                date.type());
        return errorInJson(PARSING_ERROR, "Date in wrong format!");
    }

    Json::Value options = root["options"];

    DateFormat::EStyle dstyle, tstyle;
    std::string error;

    if (!handleDateOptions(options, dstyle, tstyle, error))
        return errorInJson(PARSING_ERROR, error);

    UErrorCode status = U_ZERO_ERROR;
    const Locale& loc = Locale::getDefault();
    DateFormat* df = DateFormat::createDateTimeInstance(dstyle, tstyle, loc);

    if (!df) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::dateToString: unable to create DateFormat!");
        return errorInJson(UNKNOWN_ERROR, "Unable to create DateFormat instance!");
    }

    UnicodeString result;
    df->format(date.asDouble(), result);
    delete df;

    std::string utf8;
    result.toUTF8String(utf8);
    return resultInJson(utf8);
}

std::string GlobalizationNDK::stringToDate(const std::string& args)
{
    if (args.empty())
        return errorInJson(PARSING_ERROR, "No dateString provided!");

    Json::Reader reader;
    Json::Value root;
    bool parse = reader.parse(args, root);

    if (!parse) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::stringToDate: invalid json data: %s",
                args.c_str());
        return errorInJson(PARSING_ERROR, "Parameters not valid json format!");
    }

    Json::Value dateString = root["dateString"];
    if (!dateString.isString()) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::stringToDate: invalid dateString type: %d",
                dateString.type());
        return errorInJson(PARSING_ERROR, "dateString not a string!");
    }

    std::string dateValue = dateString.asString();
    if (dateValue.empty()) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::stringToDate: empty dateString.");
        return errorInJson(PARSING_ERROR, "dateString is empty!");
    }

    Json::Value options = root["options"];

    DateFormat::EStyle dstyle, tstyle;
    std::string error;
    if (!handleDateOptions(options, dstyle, tstyle, error))
        return errorInJson(PARSING_ERROR, error);

    const Locale& loc = Locale::getDefault();
    DateFormat* df = DateFormat::createDateTimeInstance(dstyle, tstyle, loc);

    if (!df) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::stringToDate: unable to create DateFormat instance!");
        return errorInJson(UNKNOWN_ERROR, "Unable to create DateFormat instance!");
    }

    UnicodeString uDate = UnicodeString::fromUTF8(dateValue);
    UErrorCode status = U_ZERO_ERROR;
    UDate date = df->parse(uDate, status);
    delete df;

    // Note: not sure why U_ERROR_WARNING_START is returned when parse succeeded.
    if (status != U_ZERO_ERROR && status != U_ERROR_WARNING_START) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::stringToDate: DataFormat::parse error: %d: %s",
                status, dateValue.c_str());
        return errorInJson(PARSING_ERROR, "Failed to parse dateString!");
    }

    return resultInJson(date);
}

std::string GlobalizationNDK::getDatePattern(const std::string& args)
{
    DateFormat::EStyle dstyle = DateFormat::kShort, tstyle = DateFormat::kShort;

    if (!args.empty()) {
        Json::Reader reader;
        Json::Value root;
        bool parse = reader.parse(args, root);

        if (!parse) {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::getDatePattern: invalid json data: %s",
                    args.c_str());
            return errorInJson(PARSING_ERROR, "Parameters not valid json format!");
        }

        Json::Value options = root["options"];

        std::string error;
        if (!handleDateOptions(options, dstyle, tstyle, error))
            return errorInJson(PARSING_ERROR, error);
    }

    UErrorCode status = U_ZERO_ERROR;
    SimpleDateFormat* sdf = new SimpleDateFormat(status);
    if (!sdf) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::getDatePattern: unable to create SimpleDateFormat instance: %d.",
                status);
        return errorInJson(UNKNOWN_ERROR, "Unable to create SimpleDateFormat instance!");
    }

    UnicodeString pt;
    sdf->toPattern(pt);
    std::string ptUtf8;
    pt.toUTF8String(ptUtf8);

    const TimeZone& tz = sdf->getTimeZone();

    UnicodeString tzName;
    tz.getDisplayName(tzName);
    std::string tzUtf8;
    tzName.toUTF8String(tzUtf8);

    int utc_offset = tz.getRawOffset() / 1000; // UTC_OFFSET in seconds.
    int dst_offset = tz.getDSTSavings() / 1000; // DST_OFFSET in seconds;

    delete sdf;

    return resultInJson(ptUtf8, tzUtf8, utc_offset, dst_offset);
}

enum ENamesType {
    kNamesWide,
    kNamesNarrow,
    kNameWidthCount
};

enum ENamesItem {
    kNamesMonths,
    kNamesDays,
    kNamesTypeCount
};

static bool handleNamesOptions(const Json::Value& options, ENamesType& type, ENamesItem& item, std::string& error)
{
    // This is the default value when no options provided.
    type = kNamesWide;
    item = kNamesMonths;

    if (options.isNull())
        return true;

    if (!options.isObject()) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleNamesOptions: invalid options format: %d",
                options.type());
        error = "Options is invalid!";
        return false;
    }

    Json::Value tv = options["type"];
    if (!tv.isNull()) {
        if (!tv.isString()) {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleNamesOptions: invalid type format: %d",
                    tv.type());
            error = "type is invalid!";
            return false;
        }

        std::string tstr = tv.asString();
        if (tstr.empty()) {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleNamesOptions: empty type!");
            error = "type is empty!";
            return false;
        }

        if (tstr == "narrow") {
            type = kNamesNarrow;
        } else if (tstr == "wide") {
            // Nothing to change here.
        } else {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleNamesOptions: unsupported type: %s",
                    tstr.c_str());
            error = "Unsupported type!";
            return false;
        }
    }

    Json::Value iv = options["item"];
    if (!iv.isNull()) {
        if (!iv.isString()) {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleNamesOptions: invalid item format: %d",
                    iv.type());
            error = "item is invalid!";
            return false;
        }

        std::string istr = iv.asString();
        if (istr.empty()) {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleNamesOptions: empty item!");
            error = "item is empty!";
            return false;
        }

        if (istr == "days") {
            item = kNamesDays;
        } else if (istr == "months") {
            // Nothing to change here.
        } else {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::handleNamesOptions: unsupported item: %s",
                    istr.c_str());
            error = "Unsupported item!";
            return false;
        }
    }

    return true;
}

std::string GlobalizationNDK::getDateNames(const std::string& args)
{
    ENamesType type = kNamesWide;
    ENamesItem item = kNamesMonths;

    if (!args.empty()) {
        Json::Reader reader;
        Json::Value root;
        bool parse = reader.parse(args, root);

        if (!parse) {
            slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::getDateNames: invalid json data: %s",
                    args.c_str());
            return errorInJson(PARSING_ERROR, "Parameters not valid json format!");
        }

        Json::Value options = root["options"];

        std::string error;
        if (!handleNamesOptions(options, type, item, error))
            return errorInJson(PARSING_ERROR, error);
    }

    UErrorCode status;
    DateFormatSymbols* syms = new DateFormatSymbols(status);
    if (!syms) {
        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::getDateNames: unable to create DateFormatSymbols instance: %d.",
                status);
        return errorInJson(UNKNOWN_ERROR, "Unable to create DateFormatSymbols instance!");
    }

    int count = 0;
    const UnicodeString* names;
    if (item == kNamesMonths) {
        if (type == kNamesWide)
            names = syms->getMonths(count, DateFormatSymbols::STANDALONE, DateFormatSymbols::WIDE);
        else
            names = syms->getMonths(count, DateFormatSymbols::STANDALONE, DateFormatSymbols::NARROW);
            // names = syms->getShortMonths(count);
    } else {
        if (type == kNamesWide)
            names = syms->getWeekdays(count, DateFormatSymbols::STANDALONE, DateFormatSymbols::WIDE);
        else
            names = syms->getWeekdays(count, DateFormatSymbols::STANDALONE, DateFormatSymbols::NARROW);
            // names = syms->getShortWeekdays(count);
    }

    if (!names) {
        delete syms;

        slog2f(0, ID_G11N, SLOG2_ERROR, "GlobalizationNDK::getDateNames: unable to get symbols: item: %d, type: %d.",
                item, type);
        return errorInJson(UNKNOWN_ERROR, "Unable to get symbols!");
    }

    std::string utf8Names;
    names->toUTF8String(utf8Names);

    delete syms;

    return resultInJson(utf8Names);
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
