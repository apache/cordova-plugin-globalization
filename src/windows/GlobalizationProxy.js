/*  
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at
	
	http://www.apache.org/licenses/LICENSE-2.0
	
	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

var GlobalizationError = require('./GlobalizationError');

module.exports = {
    getPreferredLanguage: function (win, fail) {
        try {
            var language = Windows.System.UserProfile.GlobalizationPreferences.languages[0];
            win({ value: language });
        } catch (e) {
            fail(e);
        }
    },

    getLocaleName: function (win, fail) {
        tryDoAction(GlobalizationProxy.GlobalizationProxy.getLocaleName, "", win, fail);
    },

    dateToString: function (win, fail, args) {
        args[0].options = args[0].options || { formatLength: "full", selector: "date and time" };        

        tryDoAction(GlobalizationProxy.GlobalizationProxy.dateToString, 
            JSON.stringify({
                date: args[0].date,
                options: args[0].options
            }), win, fail);
    },

    stringToDate: function (win, fail, args) {
        tryDoAction(GlobalizationProxy.GlobalizationProxy.stringToDate, 
            JSON.stringify({
                dateString: args[0].dateString,
                options: args[0].options
            }), win, fail);
    },

    getDateNames: function (win, fail, args) {
        tryDoAction(GlobalizationProxy.GlobalizationProxy.getDateNames, 
            JSON.stringify({
                options: args[0].options
            }), win, fail);
    },

    isDayLightSavingsTime: function (win, fail, args) {
        tryDoAction(GlobalizationProxy.GlobalizationProxy.isDayLightSavingsTime, 
            JSON.stringify({
                date: args[0].date,
            }), win, fail);
    },

    getFirstDayOfWeek: function (win, fail) {
        tryDoAction(GlobalizationProxy.GlobalizationProxy.getFirstDayOfWeek, "", win, fail);
    },

    numberToString: function (win, fail, args) {
        tryDoAction(GlobalizationProxy.GlobalizationProxy.numberToString, 
            JSON.stringify({
                number: args[0].number,
                options: args[0].options
            }), win, fail);
    },

    stringToNumber: function (win, fail, args) {
        tryDoAction(GlobalizationProxy.GlobalizationProxy.stringToNumber, 
            JSON.stringify({
                numberString: args[0].numberString,
                options: args[0].options
            }), win, fail);
    },

    getDatePattern: function (win, fail, args) {
        tryDoAction(GlobalizationProxy.GlobalizationProxy.getDatePattern, 
            JSON.stringify({
                options: args[0].options
            }), win, fail);
    },

    getNumberPattern: function (win, fail, args) {
        tryDoAction(GlobalizationProxy.GlobalizationProxy.getNumberPattern, 
            JSON.stringify({
                options: args[0].options
            }), win, fail);
    },

    getCurrencyPattern: function (win, fail, args) {
        fail("Not implemented");
    }
};

function tryDoAction(action, args, win, fail) {
    try {        
        var result = action(args);
        var obj = JSON.parse(result);
        checkForGlobalizationError(obj);
        win(obj);
    } catch (e) {
        fail(e);
    }
}

function checkForGlobalizationError(obj) {
    if (obj && 'code' in obj && 'message' in obj) {
        throw new GlobalizationError(obj.code, obj.message);
    }
}

(function init() {
    // Actually navigator.systemLanguage returns a locale
    GlobalizationProxy.GlobalizationProxy.setLocale(navigator.systemLanguage);
})();

require("cordova/exec/proxy").add("Globalization", module.exports);