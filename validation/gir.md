# Giration

## Scénario

{% set scenario_filename = '/work/reference_data_for_validation/Giration/anthineas_giration.yml' %}

## Configuration de X-DYN

Le fichier de configuration a la forme suivante :

{% set yaml_data = load(scenario_filename)%}
{{show(yaml_data)}}

{% set command_filename = '/work/reference_data_for_validation/Giration/controlled_forces_commands.yml' %}
On simule 50 secondes par pas de 0.2 secondes :
{{exec('xdyn '+ scenario_filename+ ' ' + command_filename + ' --dt 0.2  --tend 50 -o xdyn.h5')}}
{% set xdyn_res = h5('xdyn.h5', t='outputs/t', X='outputs/states/Anthineas/X', Y='outputs/states/Anthineas/Y', PSI='outputs/states/Anthineas/PSI', U='outputs/states/Anthineas/U', V='outputs/states/Anthineas/V', R='outputs/states/Anthineas/R') %}
{% set sos_res = xls('reference_data_for_validation/Giration/comparaison_giration_20150224.xlsx',sheetname='SOS') %}

## Comparaison des positions et attitudes dans le plan X,Y avec le simulateur SOS

{% set plot_xdyn_xy = prepare_plot_data(xdyn_res, name='xdyn', x = 'X', y = 'Y')%}
{% set plot_sos_xy  = prepare_plot_data(sos_res, name='SOS',x = 'x (m)', y = 'y (m)')%}
{% set graph_XY     = cartesian_graph([plot_xdyn_xy, plot_sos_xy], x='x (m)', y='y (m)', p={'grid': True, 'axis':'equal'}) %}
{{layout(size=(1,1), graphs=[(graph_XY, (0,0))], title="Position")}}

{% set plot_xdyn_tpsi = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'PSI')%}
{% set plot_sos_tpsi = prepare_plot_data(sos_res, name='SOS',x = 't( s)', y = 'psi (rad)')%}
{% set graph_PSI     = cartesian_graph([plot_xdyn_tpsi, plot_sos_tpsi], x='t(s)', y='$\psi [rad]$', p={'grid': True}) %}
{{layout(size=(1,1), graphs=[(graph_PSI, (0,0))], title="Cap")}}

## Comparaison des vitesses dans le plan X,Y avec le simulateur SOS

{% set plot_xdyn_u = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'U')%}
{% set plot_sos_u = prepare_plot_data(sos_res, name='SOS',x = 't( s)', y = 'u (m/s)')%}
{% set graph_U     = cartesian_graph([plot_xdyn_u, plot_sos_u], x='t (s)', y='U (m/s)', p={'grid': True}) %}

{% set plot_xdyn_v = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'V')%}
{% set plot_sos_v = prepare_plot_data(sos_res, name='SOS',x = 't( s)', y = 'v (m/s)')%}
{% set graph_V     = cartesian_graph([plot_xdyn_v, plot_sos_v], x='t (s)', y='V (m/s)', p={'grid': True}) %}

{% set plot_xdyn_r = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'R')%}
{% set plot_sos_r = prepare_plot_data(sos_res, name='SOS',x = 't( s)', y = 'r (rad/s)')%}
{% set graph_R     = cartesian_graph([plot_xdyn_r, plot_sos_r], x='t (s)', y='R (rad/s)', p={'grid': True}) %}
{{layout(size=(1,3), graphs=[(graph_U, (0,0)), (graph_V, (0,1)), (graph_R,(0,2))], title="Vitesses")}}

