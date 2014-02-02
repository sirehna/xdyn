function make_date(strDate)
{
    var dateParts = strDate.split("/");
    return new Date(dateParts[2], (dateParts[1] - 1), dateParts[0]);
}

function add_date(events, fieldname, fielddate, story_size, release_date)
{
    if (fielddate !== undefined)
    {

        if (events[make_date(fielddate)] === undefined)
        {
            var o = {};
            o[fieldname] = story_size;
            events[make_date(fielddate)] = o;
            events[make_date(fielddate)]["release"] = release_date;
        }
        else
        {
            events[make_date(fielddate)][fieldname] = story_size;
        }
    }
}

function get_list_of_stories()
{
    var stories = [];

    for (var i in yaml_data.stories)
    {
        var story = yaml_data.stories[i];
        stories.push(story.name);
    }
    return stories;
}


function build_events_table(yaml_data)
{
    var events = {};

    for (var i in yaml_data.stories)
    {
        var story = yaml_data.stories[i];
        var story_size = story.size;
        for (j in yaml_data.phases)
        {
            var fieldname = yaml_data.phases[j];
            var fielddate = story[fieldname];
            var release_date = make_date(story["release"]);
            add_date(events, fieldname, fielddate, story_size, release_date);
        }
    }
    events = put_all_uninitialized_events_at_0(events, yaml_data.phases);
    events = build_cumulated(events, yaml_data.phases);
    return events;
}


function put_all_uninitialized_events_at_0(events, phases)
{
    for (i in events)
    {
        var e = events[i];
        for (j in phases)
        {
            var fieldname = phases[j];
            if (typeof e[fieldname] == "undefined")
            {
                events[i][fieldname] = 0;
            }
        }
    }
    return events;
}

function get_dates(events)
{
    var dates_ = Object.keys(events);
    var dates = [];
    for (i in events)
    {
        dates.push(new Date(i));
    }
    return dates.sort(function(a, b) {
        return a - b
    });
}

function build_cumulated(events, phases)
{
    var previous = {};
    for (j in phases)
    {
        var fieldname = phases[j];
        previous[fieldname] = 0;
    }

    var dates = get_dates(events);
    for (i in dates)
    {
        var date = dates[i];
        var e = events[date];
        for (j in phases)
        {
            var fieldname = phases[j];
            if (fieldname != "release")
            {
                e[fieldname] += previous[fieldname];
                previous[fieldname] = e[fieldname];
            }
        }
    }
    return events;
}

function random_color(fieldname)
{
    if (fieldname == 'coded')
        return '#FCE94F';
    if (fieldname == 'designed')
        return '#F57900';
    if (fieldname == 'started')
        return '#8AE234';
    if (fieldname == 'integrated')
        return '#729FCF';
    if (fieldname == 'documented')
        return '#AD7FA8';
    return '#' + Math.floor(Math.random() * 16777215).toString(16);
}

function percent_complete(story, phases)
{
    var nb_complete = 0;
    for (i in phases)
    {
        var phase = phases[i];
        if (story[phase] !== undefined)
        {
            ++nb_complete;
        }
    }
    return Math.floor(100 * nb_complete / phases.length);
}

function capitaliseFirstLetter(string)
{
    return string.charAt(0).toUpperCase() + string.slice(1);
}

function make_datum(events, fieldname, dates)
{
    var data = {
        type: "area",
        name: capitaliseFirstLetter(fieldname),
        showInLegend: "true",
        color: random_color(fieldname)};
    var dataPoints = [];
    for (i in dates)
    {
        var date = dates[i];
        var e = events[date];
        dataPoints.push({x: new Date(date), y: e[fieldname]});
    }
    data.dataPoints = dataPoints;

    return data;
}

function make_cumulative_data(events, fieldnames)
{
    var data = [];
    var dates = get_dates(events);
    for (i in fieldnames)
    {
        data.push(make_datum(events, fieldnames[i], dates));
    }

    return data;
}


function last(l)
{
    return l.slice(-1)[0];
}

function get_max(events)
{
    var max_pt = 0;
    var dates = Object.keys(events);
    var last_date = last(dates);
    var e = events[last_date];
    for (fieldname in e)
    {
        if (fieldname != "release")
        {
            max_pt = Math.max(max_pt, e[fieldname]);
        }
    }
    return max_pt;
}

function sort_by_date(events)
{
    return events.sort;
}

function add_progress_bar(story_name, percent_complete, max_length)
{
    var title = document.createElement("span");
    title.setAttribute('class', "progress-title");
    title.innerHTML = capitaliseFirstLetter(story_name);
    title.setAttribute('id', story_name + '_title');

    var val = document.createElement("span");
    val.setAttribute('class', "progress-val");
    val.innerHTML = percent_complete + "%";
    val.setAttribute('id', story_name + '_val');

    var bar = document.createElement("span");
    bar.setAttribute('class', "progress-bar");
    bar.setAttribute('id', story_name + '_bar');

    var inn = document.createElement("span");
    inn.setAttribute('class', "progress-in");
    inn.style.width = percent_complete + "%";
    inn.setAttribute('id', story_name + '_inn');
    bar.appendChild(inn);

    var progress = document.createElement('div');
    progress.setAttribute('class', 'progress');
    title.setAttribute('style', 'width: ' + (Math.floor(max_length / 2) + 1) + 'em');
    progress.setAttribute('id', story_name + "_progress");
    progress.appendChild(title);
    progress.appendChild(val);
    progress.appendChild(bar);
    document.getElementById("status").appendChild(progress);
    console.log(document.body.innerHTML);
    //document.body.appendChild(progress);
}

function add_progress_bars(yaml_data, max_length)
{
    for (i in yaml_data.stories)
    {
        var story = yaml_data.stories[i];
        add_progress_bar(story.name, percent_complete(story, yaml_data.phases), max_length);
    }
}

function daysBetween(date1, date2) {
    //Get 1 day in milliseconds
    var one_day = 1000 * 60 * 60 * 24;

    // Convert both dates to milliseconds
    var date1_ms = date1.getTime();
    var date2_ms = date2.getTime();

    // Calculate the difference in milliseconds
    var difference_ms = date2_ms - date1_ms;

    // Convert back to days and return
    return Math.round(difference_ms / one_day);
}

function get_nb_of_days(events)
{
    var dates = get_dates(events);
    return daysBetween(dates[0], dates[dates.length - 1]);
}

function remove_duplicates(arr) {
    var dups = {};
    return arr.filter(function(el) {
        var hash = el.valueOf();
        var isDup = dups[hash];
        dups[hash] = true;
        return !isDup;
    });
}

function get_release_dates(yaml_data)
{
    var dates = [];
    for (i in yaml_data.stories)
    {
        var date = make_date(yaml_data.stories[i].release);
        dates.push(date);
    }
    return remove_duplicates(dates);
}

function get_cumulative_workflow(events, phases)
{
    var canvas = new CanvasJS.Chart("cumulative_workflow",
            {
                toolTip: {
                    shared: true
                },
                axisX: {
                    valueFormatString: "DD/MM/YYYY",
                    interval: Math.floor(get_nb_of_days(events) / 10),
                    intervalType: "day"
                },
                axisY: {
                    title: "Nb of story points",
                    maximum: get_max(events) * 1.5,
                    interval: Math.floor(get_max(events) / 10 + 0.5)
                },
                legend: {
                    verticalAlign: "bottom",
                    horizontalAlign: "center",
                    fontSize: 12,
                    fontFamily: "verdana",
                    fontColor: "#333",
                    borderColor: "black"
                },
                backgroundColor: "#F9F9F9",
                data: make_cumulative_data(events, phases)
            });
    return canvas;
}


function get_max_name_length(stories)
{
    var max_length = 0;
    for (i in stories)
    {
        var name = stories[i].name;
        max_length = Math.max(max_length, name.length);
    }
    return max_length;
}

function sum_work_done(ev)
{
    var s = 0;
    for (var key in ev)
    {
        if (key != 'release')
        {
            s += ev[key];
        }
    }
    return s;
}

function format(date)
{
    var day = date.getDate();
    var month = date.getMonth() + 1; //Months are zero based
    var year = date.getFullYear();
    return day + '/' + month + '/' + year;
}

function select(events, target_release, nb_of_points_to_do)
{
    selected = [];
    for (date in events)
    {
        var release_date = events[date].release;
        if (daysBetween(release_date, target_release) == 0)
        {
            var work_left = nb_of_points_to_do - sum_work_done(events[date]);
            var e = {x: new Date(date), y: work_left};
            selected.push(e);
        }
    }
    var data = {
        type: "line",
        name: format(target_release),
        showInLegend: "true",
        color: random_color(JSON.stringify(target_release))};
    data.dataPoints = selected;
    return data;
}

function to_do(yaml_data, target_release)
{
    var to_do = 0;
    var nb_of_phases = yaml_data.phases.length;
    for (i in yaml_data.stories)
    {
        var story = yaml_data.stories[i];
        if (daysBetween(make_date(story.release), target_release) == 0)
        {
            to_do += story['size'];
        }
    }
    return to_do * nb_of_phases;
}

function get_burndown(data)
{
    var name = "burndown";
    var canvas = new CanvasJS.Chart(name,
            {
                toolTip: {
                    shared: true
                },
                axisX: {
                    valueFormatString: "DD/MM/YYYY",
                    interval: 5,
                    intervalType: "day"
                },
                axisY: {
                    title: "Nb of story points",
                    maximum: 100,
                    interval: 10
                },
                legend: {
                    verticalAlign: "bottom",
                    horizontalAlign: "center",
                    fontSize: 12,
                    fontFamily: "verdana",
                    fontColor: "#333",
                    borderColor: "black"
                },
                backgroundColor: "#F9F9F9",
                data: data
            });

    return canvas;
}

function get_burndowns(yaml_data, events, releases)
{
    var data = [];
    for (var i in releases)
    {
        var release = releases[i];
        var todo = to_do(yaml_data, release);
        data.push(select(events, release, todo));
    }
    return get_burndown(data);

}

window.onload = function(started)
{
    var yaml_data = YAML.load("planning_tool.yml");
    var events = build_events_table(yaml_data);
    var burndowns = get_burndowns(yaml_data, events, get_release_dates(yaml_data));
    var cumulative_workflow = get_cumulative_workflow(events, yaml_data.phases);
    burndowns.render();
    cumulative_workflow.render();
    add_progress_bars(yaml_data, get_max_name_length(yaml_data.stories));
};
