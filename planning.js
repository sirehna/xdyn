function make_date(strDate)
{
    var dateParts = strDate.split("/");
    return new Date(dateParts[2], (dateParts[1] - 1), dateParts[0]);
}

//function add_date(events, fieldname, fielddate, story_size, release_date)
//{
//    if (fielddate !== undefined)
//    {
//
//        if (events[make_date(fielddate)] === undefined)
//        {
//            var o = {};
//            o[fieldname] = story_size;
//            events[make_date(fielddate)] = o;
//            events[make_date(fielddate)]["release"] = release_date;
//        }
//        else
//        {
//            events[make_date(fielddate)][fieldname] = story_size;
//        }
//    }
//}
//
//function get_list_of_stories()
//{
//    var stories = [];
//
//    for (var i in yaml_data.stories)
//    {
//        var story = yaml_data.stories[i];
//        stories.push(story.name);
//    }
//    return stories;
//}
//
//
//function build_events_table(yaml_data)
//{
//    var events = {};
//
//    for (var i in yaml_data.stories)
//    {
//        var story = yaml_data.stories[i];
//        var story_size = story.size;
//        for (j in yaml_data.phases)
//        {
//            var fieldname = yaml_data.phases[j];
//            var fielddate = story[fieldname];
//            var release_date = make_date(story["release"]);
//            add_date(events, fieldname, fielddate, story_size, release_date);
//        }
//    }
//    events = put_all_uninitialized_events_at_0(events, yaml_data.phases);
//    events = build_cumulated(events, yaml_data.phases);
//    return events;
//}
//
//
//function put_all_uninitialized_events_at_0(events, phases)
//{
//    for (i in events)
//    {
//        var e = events[i];
//        for (j in phases)
//        {
//            var fieldname = phases[j];
//            if (typeof e[fieldname] == "undefined")
//            {
//                events[i][fieldname] = 0;
//            }
//        }
//    }
//    return events;
//}
//
//function get_dates(events)
//{
//    var dates_ = Object.keys(events);
//    var dates = [];
//    for (i in events)
//    {
//        dates.push(new Date(i));
//    }
//    return dates.sort(function(a, b) {
//        return a - b
//    });
//}
//
//function build_cumulated(events, phases)
//{
//    var previous = {};
//    for (j in phases)
//    {
//        var fieldname = phases[j];
//        previous[fieldname] = 0;
//    }
//
//    var dates = get_dates(events);
//    for (i in dates)
//    {
//        var date = dates[i];
//        var e = events[date];
//        for (j in phases)
//        {
//            var fieldname = phases[j];
//            if (fieldname != "release")
//            {
//                e[fieldname] += previous[fieldname];
//                previous[fieldname] = e[fieldname];
//            }
//        }
//    }
//    return events;
//}
//
//function random_color(fieldname)
//{
//    if (fieldname == 'coded')
//        return '//FCE94F';
//    if (fieldname == 'designed')
//        return '//F57900';
//    if (fieldname == 'started')
//        return '//8AE234';
//    if (fieldname == 'integrated')
//        return '//729FCF';
//    if (fieldname == 'documented')
//        return '//AD7FA8';
//    return '//' + Math.floor(Math.random() * 16777215).toString(16);
//}
//
//function percent_complete(story, phases)
//{
//    var nb_complete = 0;
//    for (i in phases)
//    {
//        var phase = phases[i];
//        if (story[phase] !== undefined)
//        {
//            ++nb_complete;
//        }
//    }
//    return Math.floor(100 * nb_complete / phases.length);
//}
//
//function capitaliseFirstLetter(string)
//{
//    return string.charAt(0).toUpperCase() + string.slice(1);
//}
//
//function make_datum(events, fieldname, dates)
//{
//    var data = {
//        type: "area",
//        name: capitaliseFirstLetter(fieldname),
//        showInLegend: "true",
//        color: random_color(fieldname)};
//    var dataPoints = [];
//    for (i in dates)
//    {
//        var date = dates[i];
//        var e = events[date];
//        dataPoints.push({x: new Date(date), y: e[fieldname]});
//    }
//    data.dataPoints = dataPoints;
//
//    return data;
//}
//
//function make_cumulative_data(events, fieldnames)
//{
//    var data = [];
//    var dates = get_dates(events);
//    for (i in fieldnames)
//    {
//        data.push(make_datum(events, fieldnames[i], dates));
//    }
//
//    return data;
//}
//
//
function last(l)
{
    return l.slice(-1)[0];
}
//
//function get_max(events)
//{
//    var max_pt = 0;
//    var dates = Object.keys(events);
//    var last_date = last(dates);
//    var e = events[last_date];
//    for (fieldname in e)
//    {
//        if (fieldname != "release")
//        {
//            max_pt = Math.max(max_pt, e[fieldname]);
//        }
//    }
//    return max_pt;
//}
//
//function sort_by_date(events)
//{
//    return events.sort;
//}
//
//function add_progress_bar(story_name, percent_complete, max_length)
//{
//    var title = document.createElement("span");
//    title.setAttribute('class', "progress-title");
//    title.innerHTML = capitaliseFirstLetter(story_name);
//    title.setAttribute('id', story_name + '_title');
//
//    var val = document.createElement("span");
//    val.setAttribute('class', "progress-val");
//    val.innerHTML = percent_complete + "%";
//    val.setAttribute('id', story_name + '_val');
//
//    var bar = document.createElement("span");
//    bar.setAttribute('class', "progress-bar");
//    bar.setAttribute('id', story_name + '_bar');
//
//    var inn = document.createElement("span");
//    inn.setAttribute('class', "progress-in");
//    inn.style.width = percent_complete + "%";
//    inn.setAttribute('id', story_name + '_inn');
//    bar.appendChild(inn);
//
//    var progress = document.createElement('div');
//    progress.setAttribute('class', 'progress');
//    title.setAttribute('style', 'width: ' + (Math.floor(max_length / 2) + 1) + 'em');
//    progress.setAttribute('id', story_name + "_progress");
//    progress.appendChild(title);
//    progress.appendChild(val);
//    progress.appendChild(bar);
//    document.getElementById("status").appendChild(progress);
//    console.log(document.body.innerHTML);
//    //document.body.appendChild(progress);
//}
//
//function add_progress_bars(yaml_data, max_length)
//{
//    for (i in yaml_data.stories)
//    {
//        var story = yaml_data.stories[i];
//        add_progress_bar(story.name, percent_complete(story, yaml_data.phases), max_length);
//    }
//}
//
//function daysBetween(date1, date2) {
//    //Get 1 day in milliseconds
//    var one_day = 1000 * 60 * 60 * 24;
//
//    // Convert both dates to milliseconds
//    var date1_ms = date1.getTime();
//    var date2_ms = date2.getTime();
//
//    // Calculate the difference in milliseconds
//    var difference_ms = date2_ms - date1_ms;
//
//    // Convert back to days and return
//    return Math.round(difference_ms / one_day);
//}
//
//function get_nb_of_days(events)
//{
//    var dates = get_dates(events);
//    return daysBetween(dates[0], dates[dates.length - 1]);
//}
//
function remove_duplicates(arr) {
    var dups = {};
    return arr.filter(function(el) {
        var hash = el.valueOf();
        var isDup = dups[hash];
        dups[hash] = true;
        return !isDup;
    });
}
//
//function get_release_dates(yaml_data)
//{
//    var dates = [];
//    for (i in yaml_data.stories)
//    {
//        var date = make_date(yaml_data.stories[i].release);
//        dates.push(date);
//    }
//    return remove_duplicates(dates);
//}
//
//function get_cumulative_workflow(events, phases)
//{
//    var canvas = new CanvasJS.Chart("cumulative_workflow",
//            {
//                toolTip: {
//                    shared: true
//                },
//                axisX: {
//                    valueFormatString: "DD/MM/YYYY",
//                    interval: Math.floor(get_nb_of_days(events) / 10),
//                    intervalType: "day"
//                },
//                axisY: {
//                    title: "Nb of story points",
//                    maximum: get_max(events) * 1.5,
//                    interval: Math.floor(get_max(events) / 10 + 0.5)
//                },
//                legend: {
//                    verticalAlign: "bottom",
//                    horizontalAlign: "center",
//                    fontSize: 12,
//                    fontFamily: "verdana",
//                    fontColor: "//333",
//                    borderColor: "black"
//                },
//                backgroundColor: "//F9F9F9",
//                data: make_cumulative_data(events, phases)
//            });
//    return canvas;
//}
//
//
//function get_max_name_length(stories)
//{
//    var max_length = 0;
//    for (i in stories)
//    {
//        var name = stories[i].name;
//        max_length = Math.max(max_length, name.length);
//    }
//    return max_length;
//}
//
//function sum_work_done(ev)
//{
//    var s = 0;
//    for (var key in ev)
//    {
//        if (key != 'release')
//        {
//            s += ev[key];
//        }
//    }
//    return s;
//}
//
//function format(date)
//{
//    var day = date.getDate();
//    var month = date.getMonth() + 1; //Months are zero based
//    var year = date.getFullYear();
//    return day + '/' + month + '/' + year;
//}
//
//function select(events, target_release, nb_of_points_to_do)
//{
//    selected = [];
//    for (date in events)
//    {
//        var release_date = events[date].release;
//        if (daysBetween(release_date, target_release) == 0)
//        {
//            var work_left = nb_of_points_to_do - sum_work_done(events[date]);
//            var e = {x: new Date(date), y: work_left};
//            selected.push(e);
//        }
//    }
//    var data = {
//        type: "line",
//        name: format(target_release),
//        showInLegend: "true",
//        color: random_color(JSON.stringify(target_release))};
//    data.dataPoints = selected;
//    return data;
//}
//
//function to_do(yaml_data, target_release)
//{
//    var to_do = 0;
//    var nb_of_phases = yaml_data.phases.length;
//    for (i in yaml_data.stories)
//    {
//        var story = yaml_data.stories[i];
//        if (daysBetween(make_date(story.release), target_release) == 0)
//        {
//            to_do += story['size'];
//        }
//    }
//    return to_do * nb_of_phases;
//}
//
//function get_burndown(data)
//{
//    var name = "burndown";
//    var canvas = new CanvasJS.Chart(name,
//            {
//                toolTip: {
//                    shared: true
//                },
//                axisX: {
//                    valueFormatString: "DD/MM/YYYY",
//                    interval: 5,
//                    intervalType: "day"
//                },
//                axisY: {
//                    title: "Nb of story points",
//                    maximum: 100,
//                    interval: 10
//                },
//                legend: {
//                    verticalAlign: "bottom",
//                    horizontalAlign: "center",
//                    fontSize: 12,
//                    fontFamily: "verdana",
//                    fontColor: "//333",
//                    borderColor: "black"
//                },
//                backgroundColor: "//F9F9F9",
//                data: data
//            });
//
//    return canvas;
//}
//
//function get_burndowns(yaml_data, events, releases)
//{
//    var data = [];
//    for (var i in releases)
//    {
//        var release = releases[i];
//        var todo = to_do(yaml_data, release);
//        data.push(select(events, release, todo));
//    }
//    return get_burndown(data);
//
//}

function convert_special_characters_to_html_entities(string)
{
    var el = document.createElement("div");
    el.innerText = el.textContent = string;
    s = el.innerHTML;
    return s;
}

function insertAfter(referenceNode, newNode) {
    referenceNode.parentNode.insertBefore(newNode, referenceNode.nextSibling);
}

function add_div_to_DOM(name, node_id)
{
    var r = document.getElementById(node_id);
    var correct_name = convert_special_characters_to_html_entities(name);
    var div = document.createElement("div");
    div.setAttribute('id', correct_name);
    div.setAttribute('style', 'height: 300px; width: 60%');
    r.appendChild(div);
}

function get_delta(data)
{
    var min_val = 0;
    var max_val = 0;
    for (i in data)
    {
        var d = data[i].dataPoints;
        for (j in d)
        {
            min_val = d[j].y < min_val ? d[j].y : min_val;
            max_val = d[j].y > max_val ? d[j].y : max_val;
        }
    }
    return max_val-min_val;
}

function get_ystep(delta)
{
    var min_n = 3;
    var max_n = 7;
    var h = 10 * Math.floor(Math.log10(delta));
    
    while (delta/h < min_n)
    {
        h = h/10;
    }
    var n1 = delta/h;
    while (delta/h/5 > max_n)
    {
        h = h*10;
    }
    var n5 = delta/h/5;
    var n2 = delta/2/h;
    if (n5 >= min_n) return 5*h;
    if (n2 >= min_n) return 2*h;
    return h;
}

function plot(div_id, data, xlabel, ylabel, names, title)
{
    var ystep = get_ystep(get_delta(data));
    
    var canvas = new CanvasJS.Chart(div_id,
            {
                axisX: {
                    title: xlabel,
                    interval: 1
                },
                axisY: {
                    title: ylabel,
                    interval: ystep
                },
                title: {
                    text: title
                },
                legend: {
                    horizontalAlign: "right",
                    verticalAlign: "center",
                    fontSize: 15
                },
                backgroundColor: "//F9F9F9",
                data: data
            });
    canvas.render();
}

function get_canvas_JS_data(x,y,name,type)
{
    var dataPoints = [];
    var obj = {};
    for (i in x)
    {
        dataPoints.push({x: x[i], y: y[i]});
    }
    obj.type = type;
    obj.dataPoints = dataPoints;
    obj.showInLegend = true;
    obj.legendText = name;
    return obj;
}

function get_price(n,cost)
{
    var price = 0;
    for (i in n)
    {
        if (i != 'week')
        {
            price += n[i]*cost[i];
        }
    }
    return price;
}

function get_remaining_budget_data(data, cost)
{
    var remaining = data['budget'];
    var node = data['resource usage'];
    var x = [data['T0']];
    var y = [remaining];

    for (i in node)
    {
        var n = node[i];
        remaining -= get_price(n,cost);
        x.push(n.week);
        y.push(remaining);
    }
    return get_canvas_JS_data(x,y,data['name'],"area");
}

function get_users(data)
{
    for (i in data)
    {
        var D = data[i]['resource usage'];
        var users = [];
        for (j in D)
        {
            var d = D[j];
            for (k in d)
            {
                if (k != 'week') users.push(k);
            }
        }
    }
    return remove_duplicates(users);
}

function plot_remaining_budget(yaml_data)
{
    var data = yaml_data['budget'];
    var rdata = [];
    var users = get_users(data);
    var cost = {};
    for (i in users)
    {
        cost[users[i]] = yaml_data['prices'][yaml_data['resources'][users[i]]];
    }
    
    for (i in data)
    {
        rdata.push(get_remaining_budget_data(data[i],cost));
    }
    plot('remaining_budget', rdata, 'Week', 'Remaining budget (€)');
}

function get_resource_usage_data(node, users)
{
    var T0 = node['T0'];
    var r = node['resource usage'];
    var rdata = [];
    for (i in users)
    {
        var user = users[i];
        var x = [T0];
        var val = 0;
        var y = [val];
        for (j in r)
        {
            if (user in r[j])
            {
                val += r[j][user];
            }
            x.push(r[j].week);
            y.push(val);
        }
        rdata.push(get_canvas_JS_data(x,y,user,"line"));
    }
    return rdata;
}

function plot_resource_usage(yaml_data)
{
    var data = yaml_data['budget'];
    var users = get_users(data);
    var rdata = [];
    for (i in data)
    {
        var d = data[i];
        var divname = 'resource_usage_project_'+i;
        add_div_to_DOM(divname, 'resource_usage');
        plot(divname, get_resource_usage_data(d, users), 'Week', 'Nb of days','mkljlm');
    }
}

function get_all_dates(yaml_data, T0)
{
    var dates = [new Date(T0)];
    var stories = yaml_data.stories;
    for (i in stories)
    {
        var story = stories[i];
        for (j in story.tasks)
        {
            var task = story.tasks[j];
            for (k in task)
            {
                if ((k != 'size') && (k != 'task') && (k != 'assigned to'))
                {
                    dates.push(new Date(make_date(task[k])));
                }
            }
        }
    }
    dates = remove_duplicates(dates);
    return dates.sort(function(a,b){return a-b;});
}

function find_idx(arr, val)
{
    if (arr[0]-val>0) return 0;
    for (j = 0 ; j < arr.length-1 ; ++j)
    {
        if ((arr[j]-val<=0) && (arr[j+1]-val>0)) return j;
    }
    return arr.length-1;
}

function get_cumulated_points(phase, yaml_data, T0, dates)
{
    var stories = yaml_data.stories;
    var x = [new Date(T0)];
    var y = [0];
    for (i in stories)
    {
        var story = stories[i];
        for (j in story.tasks)
        {
            if (phase in story.tasks[j])
            {
                x.push(new Date(make_date(story.tasks[j][phase])));
                y.push(story.tasks[j].size);
            }
        }
    }
    var indices = new Array(x.length);
    for (i = 0 ; i < x.length ; ++i) indices[i] = i;
    indices.sort(function(a,b){return x[a] - x[b] < 0 ? -1 : x[a] - x[b] > 0 ? 1 : 0;});
    var new_x = [];
    var new_y = [];
    for (i in indices) new_x.push(x[indices[i]]);
    for (i in indices) new_y.push(y[indices[i]]);
    for (i = 1 ; i < new_y.length ; ++i)
    {
        new_y[i] += new_y[i-1];
    }
    var out = [];
    for (i = 0 ; i < dates.length ; ++i)
    {
        var index = find_idx(new_x, dates[i]);
        out.push(new_y[index]);
    }
    console.log('out = ' + out);
    return get_canvas_JS_data(dates,out,phase,"area");
}

function get_cumulative_workflow_data(yaml_data)
{
    var divname = 'cumulative_workflow';
    var T0 = make_date(yaml_data.start);
    var all_dates = get_all_dates(yaml_data, T0);
    console.log('all_dates = ' + JSON.stringify(all_dates));
    var backlog = get_cumulated_points('backlog', yaml_data, T0, all_dates);
    var started = get_cumulated_points('started', yaml_data, T0, all_dates);
    var done = get_cumulated_points('done', yaml_data, T0, all_dates);
    plot(divname, [backlog,started,done], 'Date', 'Nb of points');
}

function plot_cumulative_workflow(yaml_data, T0)
{
    get_cumulative_workflow_data(yaml_data, T0);
}

window.onload = function(started)
{
    var yaml_data = YAML.load("planning_tool.yml");
    var resource_usage = YAML.load("resource_usage.yml");
    plot_remaining_budget(resource_usage);
    plot_resource_usage(resource_usage);
    plot_cumulative_workflow(yaml_data);

    //var events = build_events_table(yaml_data);
    //var burndowns = get_burndowns(yaml_data, events, get_release_dates(yaml_data));
    //var cumulative_workflow = get_cumulative_workflow(events, yaml_data.phases);
    //burndowns.render();
    //cumulative_workflow.render();
    //add_progress_bars(yaml_data, get_max_name_length(yaml_data.stories));
};
