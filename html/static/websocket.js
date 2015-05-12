

$(function() {
    data = [];
    name = "cube";
    totalPoints = 300;
    function append(t,x)
    {
        if (data.length == totalPoints)
        {
            data = data.slice(1);
        }
        data.push([t,x]);
        return data;
    }

    var plot = $.plot($("#placeholder"), [ data ], { yaxis: { max: 1 }});
    latest_t = 0;

    window.WebSocket = window.WebSocket || window.MozWebSocket;
    //var websocket = new WebSocket('ws://127.0.0.1:9002');
    //var websocket = new WebSocket('ws://130.66.124.225:9002');
    var websocket = new WebSocket('ws://localhost:9002');
    websocket.onopen = function () {
        $('h1').css('color', 'green');
    };
    websocket.onerror = function () {
        $('h1').css('color', 'red');
    };
    websocket.onmessage = function (message) {
           var parsed_message = jsyaml.load(message.data);
            t = parsed_message['t'];
            x = parsed_message['x(' + name + ')']
            y = parsed_message['y(' + name + ')']
            z = parsed_message['z(' + name + ')']
           if (typeof t != 'undefined')
           {
                if (t<latest_t)
                {
                    data = [];
                }
                console.log("z = " + z);
                plot.setData([append(t,z)]);
                plot.setupGrid();
                plot.draw();
                latest_t = t;
           }
    };
            });

