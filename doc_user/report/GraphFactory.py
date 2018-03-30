# -*- coding: utf-8 -*-
import os
import matplotlib.pyplot as plt
import uuid
from collections import OrderedDict
import numpy as np


class DataPlot:
    """
    This class is just a container. It has :
    - a reference to the pandas dataframe (self.data_source)
    - a mapping between dataframe columns names and names defined by user in jinja script (self.values)
    Example :
    If we have the dataframe  df :
    | a | b | c |
    -------------
    | 1 | 4 | 7 |
    | 2 | 5 | 8 |
    | 3 | 6 | 9 |

    If we want two channels x and y, where x represents column "a" and y represents column "b", we do :
    dataplot = DataPlot(data_source=df, x="a", y="b")
    """
    def __init__(self, data_source, **requested_data):
        self.data_source = data_source
        missing_data = self._list_unavailable_data(requested_data)
        if missing_data:
            raise ValueError("Data not found : {}".format(missing_data))
        self.values = {key: value for key, value in requested_data.items()}

    def _list_unavailable_data(self, requested_data):
        return list(filter(lambda x: x not in list(self.data_source.keys()), requested_data.values()))


class PlotParams:
    known_repr = ("Cart", "Polar", "Hist", "Bar")
    number_of_values = {"Cart": (1, 3), "Polar": (2, 2), "Bar": (2, 2)}

    def __init__(self, names, grepr="Cart", name="", **kwargs):
        """
        :param names: These names will be searched in the mapping dictionnary in DataPlot class.
        :param grepr: graphical representation ("Cart",)
        :param name: displayed in legend (optional)
        """
        if grepr not in PlotParams.known_repr:
            raise ValueError("known_repr unknown, values can be : {}".format(PlotParams.known_repr))
        if not PlotParams.number_of_values[grepr][0] <= len(names) <= PlotParams.number_of_values[grepr][1]:
            raise ValueError("Wrong number of arguments given")
        self.names = names
        self.grepr = grepr
        self.name = name
        self.other_params = kwargs


class Graph:
    default_custom_params = {"grid": True, "axis": None, "xrange": None, "yrange": None, "invert_yaxis": False}
    params_linked_to_grepr = {"Cart": [],
                              "Bar": []}
    compatibility = {"Cart": ["Cart", "Bar"]}

    def __init__(self, grepr, **kwargs):
        """
        :param grepr: graphical representation ("Cart")
        :param kwargs:
        """
        self.legend = None  # TODO: USELESS ?
        self._data_plots = OrderedDict()
        self._plot_params = OrderedDict()
        self.grepr = grepr
        self.labels = {}
        self.define_labels(**kwargs)
        self.custom_params = self._define_custom_params(**kwargs)
        self.params_linked_to_grepr = self._define_params_linked_to_grepr(grepr, **kwargs)
        self.number_of_plotbar = 0

    def define_labels(self, **kwargs):
        if "x" in kwargs and "y" in kwargs and self.grepr == 'Cart':
            self.labels.update({"x": kwargs["x"]})
            self.labels.update({"y": kwargs["y"]})
        else:
            raise NotImplementedError

    def add_plot(self, data_plot, plot_params):
        if plot_params.grepr not in Graph.compatibility[self.grepr]:
            raise ValueError("All plots must have the same type of representation (for now)")
        plot_id = uuid.uuid4()
        self._data_plots.update({plot_id: data_plot})
        self._plot_params.update({plot_id: plot_params})
        if plot_params.grepr == "Bar":
            self.number_of_plotbar += 1
        return plot_id

    def _define_custom_params(self, **kwargs):
        default_params = Graph.default_custom_params.copy()
        if "p" in kwargs:
            custom_params_specified = filter(lambda x: x[0] in Graph.default_custom_params.keys(), kwargs["p"].items())
            for arg, value in custom_params_specified:
                default_params.update({arg: value})
        return default_params

    def _define_params_linked_to_grepr(self, grepr, **kwargs):
        return {param: value for param, value in kwargs.items() if param in Graph.params_linked_to_grepr[grepr]}

    @property
    def plots(self):
        return zip(self._data_plots.values(), self._plot_params.values())

    @property
    def number_of_plot(self):
        return len(self._data_plots)

    @property
    def every_plot_has_a_name(self):
        return all([plot_param.name for plot_param in self._plot_params.values()])


class GridLayout:
    id_layout = 0

    def __init__(self, size, graphs=(), title="", figsize=None):
        self.layout_size = size
        self._layout = {}
        self.title = title
        for graph, layout_coord in graphs:
            self.add_graph(layout_coord, graph)
        self.title = title
        self.id = GridLayout.id_layout
        self.figsize = figsize
        GridLayout.id_layout += 1

    def add_graph(self, layout_coord, graph):
        self._layout.update({layout_coord: graph})

    def layout_content(self, layout_key):
        return self._layout[layout_key]

    @property
    def sorted_layout_keys(self):
        return sorted(self._layout.keys())


class Image:

    def __init__(self, layout):
        self._layout = layout

    def create_image(self, path, name, image_format='png', write=True):
        fig = self.create_matplotlib_figure()
        path_image = self._write_image(image_format, name, path) if write else None
        plt.close(fig.number)
        return path_image

    def create_matplotlib_figure(self):
        fig = plt.figure(figsize=self._layout.figsize)
        fig.suptitle(self._layout.title, y=1.0)
        for layout_key in self._layout.sorted_layout_keys:
            self._draw_graph(fig=fig, graph=self._layout.layout_content(layout_key), subplot=layout_key)
        plt.tight_layout(h_pad=0.2)
        return fig

    def _write_image(self, image_format, name, path):
        path_image = os.path.join(path, "{}.{}".format(name, image_format))
        path_image = path_image.replace("..svg", ".svg")
        path_image = path_image.replace("..png", ".png")
        path_image = path_image.replace("..pdf", ".pdf")
        path_image = path_image.replace("..docx", ".docx")
        path_image = path_image.replace("..html", ".html")
        plt.savefig(path_image, dpi=250)  # , bbox_extra_artists=(t,))
        return path_image

    def _draw_graph(self, fig, graph, subplot):
        arg_loc_subplot, args = self._args_subplot(subplot, graph.grepr)
        fig.add_subplot(*arg_loc_subplot, **args)

        color = ["blue", "green", "red", "cyan", "magenta"]
        number_of_barplot_drawn = 0
        for id_plot, (data_plot, plot_params) in enumerate(graph.plots):
            if plot_params.grepr == "Cart":
                self._draw_curve_plot(fig=fig, data_plot=data_plot, plot_params=plot_params, color=color[id_plot])
            elif plot_params.grepr == "Bar":
                self._draw_bar_plot(fig=fig, data_plot=data_plot, plot_params=plot_params,
                                    number_of_plotbar=graph.number_of_plotbar, id_plot=number_of_barplot_drawn, color=color[id_plot])
                number_of_barplot_drawn += 1
            else:
                raise ValueError("Unknown kind of plot : {}".format(plot_params.grepr))

        axe = plt.subplot(*arg_loc_subplot)
        self._draw_labels(axe=axe, grepr=graph.grepr, labels=graph.labels)
        self._draw_legend(axe, graph)
        if graph.custom_params['grid']:
            plt.grid()
        if graph.custom_params['invert_yaxis']:
            ax = plt.gca()
            ax.invert_yaxis()
        if graph.custom_params['axis'] == 'equal':
            plt.axis('equal')
        if graph.custom_params['xrange']:
            axe.set_xlim([graph.custom_params['xrange'][0], graph.custom_params['xrange'][1]])
        if graph.custom_params['yrange']:
            axe.set_ylim([graph.custom_params['yrange'][0], graph.custom_params['yrange'][1]])

    def _draw_legend(self, axe, graph):
        if graph.every_plot_has_a_name:
            axe.legend(loc=2, prop={'size': 5})

    def _draw_labels(self, axe, grepr, labels):
        if grepr == "Cart":
            axe.set_xlabel(labels["x"], labelpad=0, size=10)
            axe.set_ylabel(labels["y"], labelpad=0, size=10)
        else:
            raise NotImplementedError

    def _id_of_graph(self, subplot):
        return subplot[0] * self._layout.layout_size[1] + subplot[1] + 1

    def _draw_curve_plot(self, fig, data_plot, plot_params, color):
        plt.figure(fig.number)
        data = [data_plot.data_source[data_plot.values[name]] for name in plot_params.names]
        plt.plot(*data, color=color, label=plot_params.name, **self._args_plot(plot_params))
        plt.tick_params(labelsize=5)

    def _draw_bar_plot(self, fig, data_plot, plot_params, number_of_plotbar, id_plot, color):
        plt.figure(fig.number)
        bins = data_plot.data_source["x"]
        values = data_plot.data_source["y"]
        bar_width = np.diff(bins) / number_of_plotbar
        plt.bar(left=bins[:-1] + id_plot * bar_width, height=values, width=bar_width, color=color, label=plot_params.name)

    def _args_plot(self, plot_params):
        return {}  # ex : {"color": 'b'}

    def _args_subplot(self, subplot, grepr):
        subplot_loc = self._layout.layout_size + (self._id_of_graph(subplot),)
        args = {}
        if grepr == "Polar":
            args.update({"projection": "polar"})
        return subplot_loc, args
