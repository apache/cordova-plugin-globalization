<!---
    Licensed to the Apache Software Foundation (ASF) under one
    or more contributor license agreements.  See the NOTICE file
    distributed with this work for additional information
    regarding copyright ownership.  The ASF licenses this file
    to you under the Apache License, Version 2.0 (the
    "License"); you may not use this file except in compliance
    with the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing,
    software distributed under the License is distributed on an
    "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
    KIND, either express or implied.  See the License for the
    specific language governing permissions and limitations
    under the License.
-->

# org.apache.cordova.globalization

Ce plugin obtienne des informations et effectue des opérations spécifiques aux paramètres régionaux et le fuseau horaire de l'utilisateur.

## Installation

    cordova plugin add org.apache.cordova.globalization
    

## Objets

*   GlobalizationError

## Méthodes

*   navigator.globalization.getPreferredLanguage
*   navigator.globalization.getLocaleName
*   navigator.globalization.dateToString
*   navigator.globalization.stringToDate
*   navigator.globalization.getDatePattern
*   navigator.globalization.getDateNames
*   navigator.globalization.isDayLightSavingsTime
*   navigator.globalization.getFirstDayOfWeek
*   navigator.globalization.numberToString
*   navigator.globalization.stringToNumber
*   navigator.globalization.getNumberPattern
*   navigator.globalization.getCurrencyPattern

## navigator.globalization.dateToString

Renvoie une date sous la forme d'une chaîne de caractères en tenant compte des réglages de langue et de fuseau horaire du client.

    navigator.globalization.dateToString(date, successCallback, errorCallback, options);
    

### Description

Retourne la date formatée (`String`) via la propriété `value` définie sur l'objet transmis en paramètre à la fonction `successCallback`.

La valeur du premier paramètre, nommé `date`, doit être de type `Date`.

Si une erreur survient lors du formatage de la date, la fonction `errorCallback` est exécutée et un objet `GlobalizationError` lui est passé en paramètre. Code attendu de l'erreur est`GlobalizationError.FORMATTING\_ERROR`.

Le paramètre `options` est facultatif, sa valeur par défaut est :

    {formatLength: « court », sélecteur: « date et heure »}
    

Les valeurs autorisées pour la propriété `options.formatLength` sont `short`, `medium`, `long` et `full`.

Les valeurs autorisées pour la propriété `options.selector` sont `date`, `time` et `date and time`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Si la langue du navigateur est réglée sur `fr_FR`, une fenêtre popup contenant un texte semblable à `date : 25/09/2012 16:21` est affichée (options par défaut) :

    navigator.globalization.dateToString(
        new Date(),
        function (date) { alert('date : ' + date.value + '\n'); },
        function () { alert('Une erreur est survenue lors de l\'utilisation de dateToString\n'); },
        { formatLength: 'short', selector: 'date and time' }
    );
    

### Windows Phone 8 Quirks

*   Seules les valeurs `short` et `full` sont supportées pour l'option `formatLength`.

## navigator.globalization.getCurrencyPattern

Retourne une chaîne de caractères modèle, utile pour formater et parser des devises, en tenant compte des réglages du client et du code ISO 4217 associé.

     navigator.globalization.getCurrencyPattern(currencyCode, successCallback, errorCallback);
    

### Description

Transmet le modèle demandé comme paramètre de la fonction `successCallback` sous la forme d'un objet `properties`. Cet objet contient normalement les propriétés suivantes :

*   **pattern** : le modèle permettant de formater et parser des valeurs monétaires. Les modèles suivent [Unicode Technical Standard #35][1]. *(String)*

*   **code** : le code ISO 4217 de la devise liée au modèle. *(String)*

*   **fraction** : le nombre de chiffres composant la partie décimale à utiliser lors de l'analyse et du formatage de la devise. *(Number)*

*   **rounding** : la valeur de l'incrément d'arrondi à utiliser pour l'analyse et le formatage. *(Number)*

*   **decimal** : le symbole séparant la partie entière de la partie décimale à utiliser pour l'analyse et la mise en forme. *(String)*

*   **grouping** : le symbole séparant les groupes de chiffres à utiliser pour l'analyse et la mise en forme. *(String)*

 [1]: http://unicode.org/reports/tr35/tr35-4.html

Le paramètre `currencyCode` doit être de type `String` et correspondre à l'un des codes de devise ISO 4217, "USD" par exemple.

Si une erreur survient lors de l'obtention du modèle demandé, la fonction `errorCallback` est exécutée et un objet `GlobalizationError` lui est passé en paramètre. Code attendu de l'erreur est`GlobalizationError.FORMATTING\_ERROR`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS

### Exemple

Si la langue du navigateur est réglée sur `fr_FR` et que la devise sélectionnée est le Dollar américain, cet exemple affiche une fenêtre popup contenant des résultats semblables à ceux qui suivent :

    navigator.globalization.getCurrencyPattern(
        'USD',
        function (pattern) {
            alert('pattern : '  + pattern.pattern  + '\n' +
                  'code : '     + pattern.code     + '\n' +
                  'fraction : ' + pattern.fraction + '\n' +
                  'rounding : ' + pattern.rounding + '\n' +
                  'decimal : '  + pattern.decimal  + '\n' +
                  'grouping : ' + pattern.grouping);
        },
        function () { alert('Erreur lors de l\'obtention du modèle\n'); }
    );
    

Résultats escomptés :

    pattern : #,##0.00 $
    code : USD
    fraction : 2
    rounding : 0
    decimal : ,
    grouping : 
    

## navigator.globalization.getDateNames

Retourne un tableau contenant les noms des mois ou jours de la semaine, selon le calendrier et les préférences utilisateur du client.

    navigator.globalization.getDateNames (successCallback, errorCallback, options) ;
    

### Description

Transmet le tableau de noms comme paramètre de la fonction `successCallback` sous la forme d'un objet `properties`. Cet objet contient une propriété `value` contenant un tableau (`Array`) dont les valeurs sont des chaînes de caractères (`String`). Ces chaînes de caractères sont classées dans le tableau à partir du premier mois de l'année ou du premier jour de la semaine, selon l'option choisie.

Si une erreur survient lors de l'obtention des noms, la fonction `errorCallback` est exécutée et un objet `GlobalizationError` lui est passé en paramètre. Le code d'erreur attendu dans ce cas est `GlobalizationError.UNKNOWN_ERROR`.

Le `options` paramètre est facultatif, et ses valeurs par défaut sont :

    {type: « large », item: « mois »}
    

Les valeurs autorisées pour la propriété `options.type` sont `narrow` et `wide`.

Les valeurs autorisées pour la propriété `options.item` sont `months` et `days`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Si la langue du navigateur est réglée sur `fr_FR`, cet exemple affiche une série de douze fenêtres popup, une par mois, avec contenu ressemblant à `mois : janvier`.

    navigator.globalization.getDateNames(
        function (names) {
            for (var i = 0; i < names.value.length; i++) {
                alert('mois : ' + names.value[i] + '\n');
            }
        },
        function () { alert('Erreur lors de l\'obtention des noms de mois\n'); },
        { type: 'wide', item: 'months' }
    );
    

## navigator.globalization.getDatePattern

Retourne une chaîne de caractères modèle, utile pour formater et parser des dates, en tenant compte des préférences utilisateur du client.

    navigator.globalization.getDatePattern(successCallback, errorCallback, options);
    

### Description

Transmet le modèle demandé à la fonction `successCallback`. Cet objet passé en paramètre contient les propriétés suivantes :

*   **pattern** : le modèle de date et d'heure permettant de formater et parser des dates. Les modèles suivent [Unicode Technical Standard #35][1]. *(String)*

*   **timezone** : le nom abrégé du fuseau horaire du client. *(String)*

*   **utc_offset** : la différence actuelle en secondes entre le fuseau horaire du client et le Temps universel coordonné. *(Number)*

*   **dst_offset** : la différence en secondes entre l'heure d'hiver et l'heure d'été du client. *(Number)*

Si une erreur survient lors de l'obtention du modèle demandé, la fonction `errorCallback` est exécutée et un objet `GlobalizationError` lui est passé en paramètre. Le code d'erreur attendu dans ce cas est`GlobalizationError.PATTERN_ERROR`.

Le paramètre `options` est facultatif, sa valeur par défaut est la suivante :

    {formatLength: « court », sélecteur: « date et heure »}
    

Les valeurs autorisées pour la propriété `options.formatLength` sont `short`, `medium`, `long` et `full`. Les valeurs autorisées pour la propriété `options.selector` sont `date`, `time` et `date and time`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Si la langue du navigateur est réglée sur `fr_FR`, cet exemple affiche une fenêtre popup contenant `pattern : dd/MM/yy HH:mm` :

    function checkDatePattern() {
        navigator.globalization.getDatePattern(
            function (date) { alert('pattern : ' + date.pattern + '\n'); },
            function () { alert('Erreur lors de l\'obtention du modèle\n'); },
            { formatLength: 'short', selector: 'date and time' }
        );
    }
    

### Windows Phone 8 Quirks

*   Seules les valeurs `short` et `full` sont supportés pour l'option `formatLength`.

*   La propriété `pattern` d'un modèle `date and time` retourne uniquement le plein format datetime.

*   La propriété `timezone` d'un modèle retourne le nom complet du fuseau horaire.

*   La propriété `dst_offset` d'un modèle n'est pas prise en charge et retourne donc toujours zéro.

## navigator.globalization.getFirstDayOfWeek

Retourne le premier jour de la semaine selon le calendrier et les préférences utilisateur du client.

    navigator.globalization.getFirstDayOfWeek(successCallback, errorCallback);
    

### Description

Les jours de la semaine sont numérotés à partir de 1, où 1 correspond au dimanche. Le jour demandé est transmis en paramètre à la fonction `successCallback` sous la forme d'un objet `properties`. Cet objet contient une propriété `value` dont la valeur est de type `Number`.

S'il y a une erreur, obtenir le modèle, puis le `errorCallback` s'exécute avec un `GlobalizationError` objet comme paramètre. Code attendu de l'erreur est`GlobalizationError.UNKNOWN\_ERROR`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Si la langue du navigateur est réglée sur `fr_FR`, une fenêtre popup contenant un texte semblable à `day : 2` est affichée.

    navigator.globalization.getFirstDayOfWeek(
        function (day) {alert('day : ' + day.value + '\n');},
        function () {alert('Erreur lors de l\'obtention du jour\n');}
    );
    

Retourne l'identifiant correspondant au réglage de langue actuel du client.

    navigator.globalization.getLocaleName(successCallback, errorCallback);
    

### Description

Transmet l'identifiant de langue en paramètre à la fonction `successCallback` sous la forme d'un objet `properties`. Cet objet contient une propriété `value` dont la valeur est de type `String`.

Si une erreur survient lors de l'obtention de l'identifiant, la fonction `errorCallback` est exécutée et un objet `GlobalizationError` lui est passé en paramètre. Code attendu de l'erreur est`GlobalizationError.UNKNOWN\_ERROR`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Si la langue du navigateur est réglée sur `fr_FR`, une fenêtre popup contenant le texte `locale : fr_FR` est affichée.

    navigator.globalization.getLocaleName(
        function (locale) {alert('locale : ' + locale.value + '\n');},
        function () {alert('Erreur lors de l\'obtention de l\'identifiant de langue\n');}
    );
    

### Windows Phone 8 Quirks

*   Renvoie le code à deux lettres ISO 3166 correspondant au pays ou la région actuel(le).

## navigator.globalization.getNumberPattern

Retourne une chaîne de caractères modèle, utile pour formater et parser des nombres, en tenant compte des préférences utilisateur du client.

    navigator.globalization.getNumberPattern(successCallback, errorCallback, options);
    

### Description

Transmet le modèle demandé en paramètre à la fonction `successCallback` sous la forme d'un objet `properties`. Cet objet contient les propriétés suivantes :

*   **pattern** : le modèle permettant de formater et parser des nombres. Les modèles suivent [Unicode Technical Standard #35][1]. *(String)*

*   **symbol** : le symbole à utiliser lors de la mise en forme et l'analyse, par exemple le symbole de pourcentage ou un symbole monétaire. *(String)*

*   **fraction** : le nombre de chiffres composant la partie décimale à utiliser lors de l'analyse et du formatage de nombres. *(Number)*

*   **arrondissement**: l'arrondi incrémenter pour utiliser lors de l'analyse et de mise en forme. *(Nombre)*

*   **positive** : le symbole à utiliser lors de l'analyse et du formatage des nombres positifs. *(String)*

*   **negative** : le symbole à utiliser lors de l'analyse et du formatage des nombres négatifs. *(String)*

*   **décimal**: le symbole décimal à utiliser pour l'analyse et de mise en forme. *(String)*

*   **regroupement**: le symbole de groupe à utiliser pour l'analyse et de mise en forme. *(String)*

S'il y a une erreur, obtenir le modèle, puis le `errorCallback` s'exécute avec un `GlobalizationError` objet comme paramètre. Code attendu de l'erreur est`GlobalizationError.PATTERN\_ERROR`.

Le paramètre `options` est facultatif, sa valeur par défaut est :

    {type: « decimal »}
    

Les valeurs autorisées pour la propriété `options.type` sont `decimal`, `percent` et `currency`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Si la langue du navigateur est réglée sur `fr_FR`, cet exemple affiche une fenêtre popup contenant des résultats semblables à ceux qui suivent :

    navigator.globalization.getNumberPattern(
        function (pattern) {alert('pattern : '  + pattern.pattern  + '\n' +
                                  'symbol : '   + pattern.symbol   + '\n' +
                                  'fraction : ' + pattern.fraction + '\n' +
                                  'rounding : ' + pattern.rounding + '\n' +
                                  'positive : ' + pattern.positive + '\n' +
                                  'negative : ' + pattern.negative + '\n' +
                                  'decimal : '  + pattern.decimal  + '\n' +
                                  'grouping : ' + pattern.grouping);},
        function () {alert('Erreur lors de l\'obtention du modèle\n');},
        {type:'decimal'}
    );
    

Résultats :

    pattern: #,##0.###
    symbol: 
    fraction: 0
    rounding: 0
    positive: +
    negative: -
    decimal: ,
    grouping: 
    

### Windows Phone 8 Quirks

*   La propriété `pattern` n'est pas prise en charge et retourne une chaîne vide.

*   La propriété `fraction` n'est pas prise en charge et retourne zéro.

## navigator.globalization.getPreferredLanguage

Retourne l'identifiant correspondant à la langue actuelle du client.

    navigator.globalization.getPreferredLanguage(successCallback, errorCallback);
    

### Description

Transmet l'identifiant de langue en paramètre à la fonction `successCallback` sous la forme d'un objet `properties`. Cet objet doit avoir une `value` propriété avec une `String` valeur.

Si une erreur survient lors de l'obtention de l'identifiant, la fonction `errorCallback` est exécutée et un objet `GlobalizationError` lui est passé en paramètre. Code attendu de l'erreur est`GlobalizationError.UNKNOWN\_ERROR`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Si la langue du navigateur est réglée sur `fr_FR`, une fenêtre popup contenant le texte `language : fr` est affichée :

    navigator.globalization.getPreferredLanguage(
        function (language) {alert('language : ' + language.value + '\n');},
        function () {alert('Erreur lors de l\'obtention de l\'identifiant du langage\n');}
    );
    

### Windows Phone 8 Quirks

*   Retourne le code à deux lettres ISO 639-1 pour la langue actuelle.

## navigator.globalization.isDayLightSavingsTime

Indique si l'heure d'été est en vigueur pour une date donnée en utilisant le calendrier et le fuseau horaire du client.

    navigator.globalization.isDayLightSavingsTime(date, successCallback, errorCallback);
    

### Description

Indique si l'heure d'été est en vigueur ou non en transmettant un objet `properties` en paramètre de la fonction `successCallback`. Cet objet contient une propriété `dst` dont la valeur est de type `Boolean`. Ainsi, `true` indique que l'heure d'été est en vigueur à la date donnée, au contraire `false` indique qu'elle ne l'est pas.

Le paramètre obligatoire `date` doit être de type `Date`.

Si une erreur survient lors de la lecture de la date, la fonction `errorCallback` est exécutée. Le code d'erreur attendu dans ce cas est`GlobalizationError.UNKNOWN_ERROR`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Pendant l'été et si l'heure d'été est activée sur le fuseau horaire actuel du navigateur, une fenêtre popup contenant `dst : true` est affichée :

    navigator.globalization.isDayLightSavingsTime(
        new Date(),
        function (date) {alert('dst : ' + date.dst + '\n');},
        function () {alert('Erreur lors de l\'obtention de la valeur de dst\n');}
    );
    

## navigator.globalization.numberToString

Renvoie un nombre formaté dans une chaîne de caractères en tenant compte des préférences utilisateur du client.

    navigator.globalization.numberToString(number, successCallback, errorCallback, options);
    

### Description

Transmet le nombre formaté en paramètre à la fonction `successCallback` sous la forme d'un objet `properties`. Cet objet doit avoir une `value` propriété avec une `String` valeur.

Si une erreur survient lors du formatage du nombre, la fonction `errorCallback` est exécutée et un objet `GlobalizationError` lui est passé en paramètre. Code attendu de l'erreur est`GlobalizationError.FORMATTING\_ERROR`.

Le `options` paramètre est facultatif, et ses valeurs par défaut sont :

    {type: « decimal »}
    

Le `options.type` peut être « decimal », « % » ou « monnaie ».

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Si la langue du navigateur est réglée sur `fr_FR`, une fenêtre popup contenant `number : 3,142` est affichée :

    navigator.globalization.numberToString(
        3.1415926,
        function (number) {alert('number : ' + number.value + '\n');},
        function () {alert('Erreur lors du formatage du nombre\n');},
        {type:'decimal'}
    );
    

## navigator.globalization.stringToDate

Retourne un objet date construit à partir d'une date contenue dans une chaîne de caractères, en tenant compte des préférences utilisateur, du calendrier et du fuseau horaire du client.

    navigator.globalization.stringToDate(dateString, successCallback, errorCallback, options);
    

### Description

Transmet la date demandée en paramètre à la fonction successCallback sous la forme d'un objet `properties`. Cet objet contient les propriétés suivantes :

*   **year** : le nombre à quatre chiffres représentant l'année. *(Number)*

*   **month** : le numéro du mois compris entre 0 et 11. *(Number)*

*   **day** : le numéro du jour compris entre 1 et 31. *(Number)*

*   **hour** : l'heure comprise entre 0 et 23. *(Number)*

*   **minute** : le nombre de minutes compris entre 0 et 59. *(Number)*

*   **second** : le nombre de secondes compris entre 0 et 59. *(Number)*

*   **millisecond** : le nombre de millisecondes compris entre 0 et 999, pas disponible pour toutes les plates-formes. *(Number)*

Le paramètre `dateString` doit être de type `String`.

Le paramètre `options` est facultatif, sa valeur par défaut est :

    {formatLength: « court », sélecteur: « date et heure »}
    

Le `options.formatLength` peut être `short` , `medium` , `long` , ou `full` . Le `options.selector` peut être `date` , `time` ou`date and
time`.

Si une erreur survient lors de l'analyse de la chaîne de caractères, la fonction `errorCallback` est exécutée et un objet `GlobalizationError` lui est passé en paramètre. Le code d'erreur prévu dans ce cas est `GlobalizationError.PARSING_ERROR`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Si la langue du navigateur est réglée sur `fr_FR`, une fenêtre popup contenant `month : 8, day : 25, year : 2012` est affichée. Notez que l'entier faisant référence au numéro du mois est différent de celui présent dans la chaîne parsée, ceci car il représente un index de tableau.

    navigator.globalization.stringToDate(
        '25/09/2012',
        function (date) {alert('month : ' + date.month + ',' +
                               ' day : '  + date.day + ',' +
                               ' year : ' + date.year + '\n');},
        function () {alert('Erreur lors de l\'obtention de la date\n');},
        {selector: 'date'}
    );
    

### Windows Phone 8 Quirks

*   Le `formatLength` prend en charge uniquement l'option `short` et `full` valeurs.

## navigator.globalization.stringToNumber

Retourne un nombre à partir d'une chaîne de caractères contenant un nombre formaté, en tenant compte des préférences utilisateur du client.

    navigator.globalization.stringToNumber(string, successCallback, errorCallback, options);
    

### Description

Transmet le nombre demandé en paramètre à la fonction `successCallback` sous la forme d'un objet `properties`. Cet objet contient une propriété `value` dont la valeur est de type `Number`.

Si une erreur survient lors de l'analyse de la chaîne de caractères, la fonction `errorCallback` est exécutée et un objet `GlobalizationError` lui est passé en paramètre. Le code d'erreur prévu dans ce cas est `GlobalizationError.PARSING_ERROR`.

Le `options` paramètre est facultatif et par défaut les valeurs suivantes :

    {type: « decimal »}
    

Le `options.type` peut être `decimal` , `percent` , ou`currency`.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS
*   Windows Phone 8

### Exemple

Si la langue du navigateur est réglée sur `fr_FR`, une fenêtre popup contenant `number : 1234.56` est affichée :

    navigator.globalization.stringToNumber(
        '1234,56',
        function (number) {alert('number : ' + number.value + '\n');},
        function () {alert('Erreur lors de l\'obtention du nombre\n');},
        {type:'decimal'}
    );
    

## GlobalizationError

Un objet représentant une erreur de l'API Globalization.

### Propriétés

*   **code** : un des codes suivants décrivant le type d'erreur rencontrée *(Number)* 
    *   GlobalizationError.UNKNOWN_ERROR : 0
    *   GlobalizationError.FORMATTING_ERROR : 1
    *   GlobalizationError.PARSING_ERROR : 2
    *   GlobalizationError.PATTERN_ERROR : 3
*   **message** : un message texte comprenant l'explication de l'erreur et/ou des détails à son sujet. *(String)*

### Description

Cet objet est créé et remplie par Cordova, puis transmis à une fonction callback en cas d'erreur.

### Plates-formes prises en charge

*   Amazon Fire OS
*   Android
*   iOS

### Exemple

Lorsque la fonction callback d'erreur suivante est exécutée, une fenêtre popup contenant par exemple `code : 3` et `message :` est affichée.

    function errorCallback(error) {
        alert('code : ' + error.code + '\n' +
              'message : ' + error.message + '\n');
    };