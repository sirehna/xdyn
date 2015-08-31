
$(function() {
    ship_name = "";
    var_to_plot = "z";
    var plotter = get_plotter();
    var Tmax = 0;
    var gui_elements = ["stlfilechooser", "solver", "outputs", "run_button"];

$('form#filechooser').submit(function(event) {
    event.preventDefault(); // Otherwise it reloads the page
    url = $(this).attr( 'action' );
    var formData = new FormData($(this)[0]);
    Tmax = $('#durationduration').val();
    /* Send the data using ajax */
    $.ajax(
    {
        url: url,
        data:formData,
        type:'POST',
    processData: false, contentType: false
    }
);

});

    $("#yaml_upload").change(function() {
        if(this.files.length)
        {
            var reader = new FileReader();

            reader.onload = function(e)
            {
                var parsed_message = jsyaml.load(e.target.result);
                if (parsed_message['bodies'])
                {
                    var mesh = parsed_message['bodies'][0]['mesh'];
                    ship_name = parsed_message['bodies'][0]['name'];
                    if (mesh)
                    {
                        show("stlfilechooser");
                    }
                    else
                    {
                        hide("stlfilechooser");
                        $.map(all_after(gui_elements, "stlfilechooser"), show);
                    }
                }
            };

            reader.readAsBinaryString(this.files[0]);
        }
        else
        {
            hide("stlfilechooser");
            $.map(all_after(gui_elements, "stlfilechooser"), show);
        }
        show("yaml_file_name");
    });
    
    $("#stl_upload").change(function() {
        $.map(all_after(gui_elements, "stlfilechooser"), show);
    });

    
    $.map(gui_elements, hide);
    hide("graph");
    hide("graph_title");
    window.WebSocket = window.WebSocket || window.MozWebSocket;
    var address = $('#websocket_address').html();
    var websocket = new WebSocket(address);
    websocket.onopen = function () {
        $('h1').css('color', '#1e88e5');
        show("filechooser");
    };

    websocket.onerror = function () {
        $('h1').css('color', 'red');
        register_connection_state("disconnected");
    };
    websocket.onmessage = function (message) {
        show("graph");
        show("graph_title");
        try
        {
            var parsed_message = jsyaml.load(message.data);
            plotter(parsed_message, ship_name, var_to_plot);
        }
        catch(err)
        {
            console.log(err.message)
            console.log('Cannot parse ' + message.data)
        }
    };
});
