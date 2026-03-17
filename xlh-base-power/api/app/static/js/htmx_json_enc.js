htmx.defineExtension('json-enc', {
    onEvent: function (name, evt) {
        if (name === "htmx:configRequest") {
            evt.detail.headers['Content-Type'] = "application/json";
        }
    },

    encodeParameters : function(xhr, parameters, elt) {
        xhr.overrideMimeType('text/json');
        return (JSON.stringify(parameters));
    }
});
// https://www.submitjson.com/blog/htmx-post-json
// https://htmx.org/extensions/#defining