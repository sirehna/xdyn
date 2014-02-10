function make_date(strDate)
{
    var dateParts = strDate.split("/");
    return new Date(dateParts[2], (dateParts[1] - 1), dateParts[0]);
}

function capitaliseFirstLetter(s)
{
    return s.charAt(0).toUpperCase() + s.slice(1);
}

function last(l)
{
    return l.slice(-1)[0];
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
    document.body.appendChild(progress);
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

function get_max_name_length(stories)
{
    var max_length = 0;
    for (i in stories)
    {
        var name = stories[i].story;
        max_length = Math.max(max_length, name.length);
    }
    return max_length;
}

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
    if (h == 0) return 1;
    
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
                    interval: 1,
					valueFormatString: "DDD-MMM-DD" ,
					labelAngle: -50
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
        if (i != 'week ending on')
        {
            price += n[i]*cost[i];
        }
    }
    return price;
}

function get_remaining_budget_data(project, users, cost)
{
    var remaining = project['budget'];
    var node = project['resource usage'];
    var x = [make_date(project['T0'])];
    var y = [remaining];
	
    var price = {};
    for (i in users)
    {
        price[users[i]] = project['prices'][cost[users[i]]];
    }

    for (i in node)
    {
        var n = node[i];
        remaining -= get_price(n,price);
        x.push(make_date(n['week ending on']));
        y.push(remaining);
    }
    return get_canvas_JS_data(x,y,project['name'],"line");
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
                if (k != 'week ending on') users.push(k);
            }
        }
    }
    return remove_duplicates(users);
}

function plot_remaining_budget(yaml_data)
{
    var projects = yaml_data['projects'];
    var rdata = [];
    var users = get_users(projects);
    
    for (i in projects)
    {
        rdata.push(get_remaining_budget_data(projects[i],users,yaml_data['resources']));
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
            x.push(make_date(r[j]['week ending on']));
            y.push(val);
        }
        rdata.push(get_canvas_JS_data(x,y,user,"line"));
    }
    return rdata;
}

function plot_resource_usage(yaml_data)
{
    var projects = yaml_data['projects'];
    var users = get_users(projects);
    var rdata = [];
    for (i in projects)
    {
        var project = projects[i];
        var divname = 'resource_usage_project_'+i;
        add_div_to_DOM(divname, 'resource_usage');
        plot(divname, get_resource_usage_data(project, users), 'Week', 'Nb of days','mkljlm');
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
                if ((k != 'size') && (k != 'task') && (k != 'assigned to') && (k != 'type'))
                {
                    dates.push(new Date(make_date(task[k])));
                }
            }
        }
    }
    dates = remove_duplicates(dates);
    return dates.sort(function(a,b){return a-b;});
}

function cumulate(x)
{
    for (i = 1 ; i < x.length ; ++i)
    {
        x[i] += x[i-1];
    }
	return x;
}

function select_tasks_by_user(tasks,user)
{
	var ret = [];
	for (j in story.tasks)
	{
		if (tasks[j]['assigned to'] == user)
		{
			ret.push(tasks[j]);	
		}
	}
	return ret;
}

function select_tasks_by_type(tasks,type)
{
	var ret = [];
	for (j in tasks)
	{
		if (tasks[j].type == type)
		{
			ret.push(tasks[j]);	
		}
	}
	return ret;
}

function select_stories_by_date(stories, date)
{
	var ret = [];
	for (i in stories)
	{
		var story = stories[i];
		if (make_date(story['due by'])-date == 0)
		{
			ret.push(story);
		}
	}
	return ret;
}

function convert_stories_to_tasks(stories)
{
	var tasks = [];
	for (i in stories)
	{
		for (j in stories[i].tasks)
		{
			tasks.push(stories[i].tasks[j]);
		}
	}
	return tasks;
}

function select_tasks_by_phase(tasks, phase)
{
	var ret = [];
	for (j in tasks)
	{
		if (phase in tasks[j])
		{
			ret.push(tasks[j]);
		}
	}
	return ret;
}

function make_data(tasks,phase,T0)
{
	var data = {x:[new Date(T0)], y:[0]};
    for (i in tasks)
    {
        var task = tasks[i];
        if (typeof task[phase] != 'undefined')
        {
            data.x.push(new Date(make_date(task[phase])));
            data.y.push(task.size);
        }
    }
	return data;
}

function sort(data)
{
	var new_data = {x: [], y:[]};
    var indices = new Array(data.x.length);
    for (i = 0 ; i < data.x.length ; ++i) indices[i] = i;
    indices.sort(function(a,b){return data.x[a] - data.x[b] < 0 ? -1 : data.x[a] - data.x[b] > 0 ? 1 : 0;});
    for (i in indices) new_data.x.push(data.x[indices[i]]);
    for (i in indices) new_data.y.push(data.y[indices[i]]);
	return new_data;
}

function interpolate(data, dates)
{
	var new_data = {x: dates, y:[]};
	var j = 0;
    for (i = 0 ; i < dates.length ; ++i)
    {
        new_data.y.push(0);
        for (j = 0 ; j < data.x.length ; ++j)
        {
            if (data.x[j]-dates[i]<=0)
            {
                new_data.y[i] = data.y[j];
            }
        }
    }
	return new_data;
}

function get_cumulated_points(phase, tasks, T0, dates, canvas_type, legend)
{
    var legend = typeof(legend)=='undefined' ? phase : legend;
	var data = sort(make_data(tasks, phase, T0));
	data.y = cumulate(data.y);
	data = interpolate(data, dates);
    return get_canvas_JS_data(data.x,data.y,legend,canvas_type);
}

function plot_cumulative_workflow(yaml_data,T0)
{
    var divname = 'cumulative_workflow';
    var all_dates = get_all_dates(yaml_data, T0);
    var tasks = convert_stories_to_tasks(yaml_data.stories);
    
    var backlog = get_cumulated_points('backlog', select_tasks_by_phase(tasks, 'backlog'), T0, all_dates, "area");
    var started = get_cumulated_points('started', select_tasks_by_phase(tasks, 'started'), T0, all_dates, "area");
    var done = get_cumulated_points('done', select_tasks_by_phase(tasks, 'done'), T0, all_dates, "area");
    plot(divname, [backlog,started,done], 'Date', 'Nb of points');
}

function plot_cumulative_workflow_by_type(yaml_data,T0)
{
    var stuff_to_plot = [];
    var divname = 'cumulative_workflow_by_type';
    var all_dates = get_all_dates(yaml_data, T0);
    var tasks = convert_stories_to_tasks(yaml_data.stories);
    var interface_ = get_cumulated_points('done', select_tasks_by_type(tasks,'interface'), T0, all_dates, "line", 'Interface');
    var kernel = get_cumulated_points('done', select_tasks_by_type(tasks,'kernel'), T0, all_dates, "line", 'Kernel');
    var model = get_cumulated_points('done', select_tasks_by_type(tasks,'model'), T0, all_dates, "line", 'Model');
    var internal = get_cumulated_points('done', select_tasks_by_type(tasks,'internal'), T0, all_dates, "line", 'Internal');
    plot(divname, [interface_,kernel,model,internal], 'Date', 'Nb of points');
}

function get_release_dates(yaml_data)
{
	var release_dates = [];
	for (i in yaml_data.stories)
	{
		release_dates.push(new Date(make_date(yaml_data.stories[i]['due by'])));
	}
	release_dates = remove_duplicates(release_dates);
	return release_dates.sort(function(a,b) {return a-b;});	
}

function get_total_nb_of_points(tasks)
{
	var total_size = 0;
	for (i in tasks)
	{
		total_size += tasks[i].size;
	}
	return total_size;
}

function convert_to_burndown(total_nb_of_points, data)
{
	if (data.y.length > 0)
	{
		data.y[0] = total_nb_of_points - data.y[0];
		for (var i = 1 ; i < data.y.length ; ++i)
		{
			data.y[i] = data.y[i-1] - data.y[i];
		}
	}
	return data;
}

function get_burndown_points(tasks, T0, name)
{
	var nb_of_points = get_total_nb_of_points(tasks);
	var data = sort(make_data(select_tasks_by_phase(tasks, 'done'), 'done', T0));
	data = convert_to_burndown(nb_of_points, data);
	return get_canvas_JS_data(data.x,data.y,name,"line");
}

function print_date(d)
{
	var curr_date = d.getDate();
	var curr_month = d.getMonth() + 1; //Months are zero based
	var curr_year = d.getFullYear();
	var ret = curr_date + '-' + curr_month + '-' + curr_year;
	return ret;
}

function plot_burndown_by_release(yaml_data,T0)
{
	var release_dates = get_release_dates(yaml_data);
	var points = [];
	for (i in release_dates)
	{
		var date = release_dates[i];
		var tasks = convert_stories_to_tasks(select_stories_by_date(yaml_data.stories, date));
		points.push(get_burndown_points(tasks,T0,print_date(date)));
	}
    plot('burndown', points, 'Date', 'Remaining points');
}

function get_percentage_completed(story)
{
	var size = 0;
	var completed = 0;
	for (i in story.tasks)
	{
		var task = story.tasks[i];
		size += task.size;
		if ('done' in task) completed += task.size;
	}
    if (size == 0) return 0;
    return Math.floor(100*completed/size+0.5);
}

function progress_bars(stories)
{
    var max_length = 30;//get_max_name_length(stories);
    for (i in stories)
    {
        var story = stories[i];
        var percent_complete = get_percentage_completed(story);
        add_progress_bar(story.story, percent_complete, max_length);
    }
}

window.onload = function(started)
{
    var yaml_data = YAML.load("planning_tool.yml");
    var resource_usage = YAML.load("resource_usage.yml");
    var T0 = make_date(yaml_data.start);
    plot_remaining_budget(resource_usage);
    plot_resource_usage(resource_usage);
    plot_cumulative_workflow(yaml_data,T0);
	plot_burndown_by_release(yaml_data, T0);
    progress_bars(yaml_data.stories);
    plot_cumulative_workflow_by_type(yaml_data,T0)
};
