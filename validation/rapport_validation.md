{% macro plot_position_and_attitude(xdyn_res, sos_res) %}
{% set plot_x_xdyn = prepare_plot_data(xdyn_res, name='xdyn',x = 't', y = 'x')%}
{% set plot_y_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'y')%}
{% set plot_z_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'z')%}
{% set plot_phi_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'phi')%}
{% set plot_theta_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'theta')%}
{% set plot_psi_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'psi')%}
{% set plot_x_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y = 'x (m)')%}
{% set plot_y_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y = 'y (m)')%}
{% set plot_z_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y = 'z (m)')%}
{% set plot_phi_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y = 'phi (rad)')%}
{% set plot_theta_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y = 'theta (rad)')%}
{% set plot_psi_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y = 'psi (rad)')%}
{% set x_graph     = cartesian_graph([plot_x_xdyn, plot_x_sos], x='t (s)', y='$X [m]$', p={'grid': True}) %}
{% set y_graph     = cartesian_graph([plot_y_xdyn, plot_y_sos], x='t (s)', y='$Y [m]$', p={'grid': True}) %}
{% set z_graph     = cartesian_graph([plot_z_xdyn, plot_z_sos], x='t (s)', y='$Z [m]$', p={'grid': True}) %}
{% set phi_graph   = cartesian_graph([plot_phi_xdyn, plot_phi_sos], x='t (s)', y='$\phi$ [rad]', p={'grid': True}) %}
{% set theta_graph = cartesian_graph([plot_theta_xdyn, plot_theta_sos], x='t(s)', y='$\\theta$ [rad]', p={'grid': True}) %}
{% set psi_graph   = cartesian_graph([plot_psi_xdyn, plot_psi_sos], x='t (s)', y='$\psi$ [rad]', p={'grid': True}) %}
{{layout(size=(2,3), graphs=[(x_graph, (0,0)), (y_graph, (0,1)), (z_graph, (0,2)), (phi_graph, (1,0)), (theta_graph, (1,1)), (psi_graph, (1,2))], title="Attitude navire")}}
{% endmacro %}

{%macro plot_speed(xdyn_res, sos_res)%}
{%set plot_U_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='U')%}
{%set plot_V_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='V')%}
{%set plot_W_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='W')%}
{%set plot_P_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='P')%}
{%set plot_Q_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='Q')%}
{%set plot_R_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='R')%}
{%set plot_U_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='u (m/s)')%}
{%set plot_V_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='v (m/s)')%}
{%set plot_W_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='w (m/s)')%}
{%set plot_P_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='p (rad/s)')%}
{%set plot_Q_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='q (rad/s)')%}
{%set plot_R_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='r (rad/s)')%}
{% set U_graph = cartesian_graph([plot_U_xdyn, plot_U_sos], x='t (s)', y='$U [m/s]$', p={'grid': True}) %}
{% set V_graph = cartesian_graph([plot_V_xdyn, plot_V_sos], x='t (s)', y='$V [m/s]$', p={'grid': True}) %}
{% set W_graph = cartesian_graph([plot_W_xdyn, plot_W_sos], x='t (s)', y='$W [m/s]$', p={'grid': True}) %}
{% set P_graph = cartesian_graph([plot_P_xdyn, plot_P_sos], x='t (s)', y='$P$ [rad/s]', p={'grid': True}) %}
{% set Q_graph = cartesian_graph([plot_Q_xdyn, plot_Q_sos], x='t (s)', y='$Q$ [rad/s]', p={'grid': True}) %}
{% set R_graph = cartesian_graph([plot_R_xdyn, plot_R_sos], x='t (s)', y='$R$ [rad/s]', p={'grid': True}) %}
{{layout(size=(2,3), graphs=[(U_graph, (0,0)), (V_graph, (0,1)), (W_graph, (0,2)), (P_graph, (1,0)), (Q_graph, (1,1)), (R_graph, (1,2))], title="Vitesses")}}
{%endmacro%}

# Extinction en pilonnement
## Scénario
{% set scenario_filename = 'reference_data_for_validation/Extinctions_simu/anthineas_amortissementPilonnement_sans_controleur.yml' %}


## Configuration de X-DYN
Le fichier de configuration a la forme suivante :

{%set yaml_data = load(scenario_filename)%}
{{show(yaml_data)}}

On simule 50 secondes par pas de 0.2 seconde :
{{exec('xdyn '+scenario_filename+' --dt 0.2 --tend 50 -o xdyn.h5')}}

## Comparaison avec le simulateur SOS-stabilité

### Déplacements et attitude
{% set xdyn_res = h5('xdyn.h5', t='outputs/t', x='outputs/states/Anthineas/X', y='outputs/states/Anthineas/Y', z='outputs/states/Anthineas/Z', phi='outputs/states/Anthineas/PHI', theta='outputs/states/Anthineas/THETA', psi='outputs/states/Anthineas/PSI',
                                U='outputs/states/Anthineas/U', V='outputs/states/Anthineas/V', W='outputs/states/Anthineas/W', P='outputs/states/Anthineas/P', Q='outputs/states/Anthineas/Q', R='outputs/states/Anthineas/R') %}
{% set sos_res = xls('reference_data_for_validation/Extinctions_simu/comparaison_extinctionPilonnement_zIni_2_simulateur_SOS.xlsx', sheetname='SOS') %}
{{plot_position_and_attitude(xdyn_res, sos_res)}}

### Vitesses
{{plot_speed(xdyn_res, sos_res)}}

# Extinction en roulis
## Scénario

{% set scenario_filename = 'reference_data_for_validation/Extinctions_simu/anthineas_amortissementRoulis_sans_controleur.yml' %}

## Configuration de X-DYN
Le fichier de configuration a la forme suivante :

{% set yaml_data = load(scenario_filename) %}
{{show(yaml_data)}}

On simule 50 secondes par pas de 0.2 secondes :
{{exec('xdyn '+scenario_filename + ' --dt 0.2 --tend 50 -o xdyn.h5')}}
{% set xdyn_res = h5('xdyn.h5', t='outputs/t', x='outputs/states/Anthineas/X', y='outputs/states/Anthineas/Y', z='outputs/states/Anthineas/Z', phi='outputs/states/Anthineas/PHI', theta='outputs/states/Anthineas/THETA', psi='outputs/states/Anthineas/PSI',
                                U='outputs/states/Anthineas/U', V='outputs/states/Anthineas/V', W='outputs/states/Anthineas/W', P='outputs/states/Anthineas/P', Q='outputs/states/Anthineas/Q', R='outputs/states/Anthineas/R') %}
{% set sos_res = xls('reference_data_for_validation/Extinctions_simu/comparaison_extinctionRoulis_phiIni_10_simulateur_SOS.xlsx', sheetname='SOS') %}

## Comparaison avec le simulateur SOS-stabilité

### Déplacements et attitude
{{plot_position_and_attitude(xdyn_res, sos_res)}}



### Vitesses
{{plot_speed(xdyn_res, sos_res)}}

# Extinction en tangage
## Scénario
{% set scenario_filename = 'reference_data_for_validation/Extinctions_simu/anthineas_amortissementTangage_sans_controleur.yml' %}

## Configuration de X-DYN
Le fichier de configuration a la forme suivante :

{% set yaml_data = load(scenario_filename) %}
{{show(yaml_data)}}

On simule 50 secondes par pas de 0.2 secondes :
{{exec('xdyn '+ scenario_filename+ ' --dt 0.2  --tend 50 -o xdyn.h5')}}
{% set xdyn_res = h5('xdyn.h5', t='outputs/t', x='outputs/states/Anthineas/X', y='outputs/states/Anthineas/Y', z='outputs/states/Anthineas/Z', phi='outputs/states/Anthineas/PHI', theta='outputs/states/Anthineas/THETA', psi='outputs/states/Anthineas/PSI',
                                U='outputs/states/Anthineas/U', V='outputs/states/Anthineas/V', W='outputs/states/Anthineas/W', P='outputs/states/Anthineas/P', Q='outputs/states/Anthineas/Q', R='outputs/states/Anthineas/R') %}
{% set sos_res = xls('reference_data_for_validation/Extinctions_simu/comparaison_extinctionTangage_thetaIni_10_simulateur_SOS.xlsx', sheetname='SOS') %}


## Comparaison avec le simulateur SOS-stabilité

### Déplacements et attitude
{{plot_position_and_attitude(xdyn_res, sos_res)}}

### Vitesses
{{plot_speed(xdyn_res, sos_res)}}


# Accélération

## Scénario
{% set scenario_filename = 'reference_data_for_validation/AccelerationDeceleration/anthineas_acceleration.yml' %}
{% set command_filename = 'reference_data_for_validation/AccelerationDeceleration/controlled_forces_commands_acceleration.yml' %}

## Configuration de X-DYN
Le fichier de configuration a la forme suivante :

{% set yaml_data = load(scenario_filename)%}
{{show(yaml_data)}}

On simule 50 secondes par pas de 0.2 secondes :
{{exec('xdyn '+ scenario_filename+ ' '+command_filename+' --dt 0.2  --tend 50 -o xdyn.h5')}}
{% set xdyn_res = h5('xdyn.h5', t='outputs/t', U='outputs/states/Anthineas/U') %}
{% set sos_trials = xls('reference_data_for_validation/AccelerationDeceleration/AnalyseEssaisErdre_201212.xlsx', sheetname='Accélération', skiprows=3) %}

## Comparaison avec les essais SOS-stabilité

{% set plot_U_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'U')%}
{% set plot_U_trial1 = prepare_plot_data(sos_trials, name='essais 1',x = 'Temps recalé (s)', y = 'Vitesse (m/s)')%}
{% set plot_U_trial2 = prepare_plot_data(sos_trials, name='essais 2',x = 'Temps recalé (s).1', y = 'Vitesse (m/s).1')%}
{% set plot_U_trial3 = prepare_plot_data(sos_trials, name='essais 3',x = 'Temps recalé (s).2', y = 'Vitesse (m/s).2')%}

{% set graph_U     = cartesian_graph([plot_U_xdyn, plot_U_trial1, plot_U_trial2, plot_U_trial3], x='t (s)', y='$U [m/s]$', p={'grid': True}) %}
{{layout(size=(1,1), graphs=[(graph_U, (0,0))], title="Accélération")}}

## Comparaison avec le simulateur SOS-stabilité

{% set plot_U_sos_sim = prepare_plot_data(sos_trials, name='simulateur SOS',x = 'Temps recalé (s).3', y = 'Vitesse (m/s).3')%}
{% set graph_U     = cartesian_graph([plot_U_xdyn, plot_U_sos_sim], x='t (s)', y='$U [m/s]$', p={'grid': True}) %}
{{layout(size=(1,1), graphs=[(graph_U, (0,0))], title="Accélération")}}

# Décélération

## Scénario
{% set scenario_filename = 'reference_data_for_validation/AccelerationDeceleration/anthineas_deceleration.yml' %}

## Configuration de X-DYN
Le fichier de configuration a la forme suivante :

{% set yaml_data = load(scenario_filename)%}
{{show(yaml_data)}}

On simule 50 secondes par pas de 0.2 secondes :
{{exec('xdyn '+ scenario_filename+ ' --dt 0.2  --tend 50 -o xdyn.h5')}}
{% set xdyn_res = h5('xdyn.h5', t='outputs/t', U='outputs/states/Anthineas/U') %}
{% set sos_trials = xls('reference_data_for_validation/AccelerationDeceleration/AnalyseEssaisErdre_201212.xlsx', sheetname='Décélération', skiprows=3) %}
{% set plot_U_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'U')%}

## Comparaison avec les essais maquette SOS-stabilité échelle 1/11e
{% set plot_U_trial1 = prepare_plot_data(sos_trials, name='Essai maquette 1',x = 'Temps recalé (s)', y = 'Vitesse (m/s)')%}
{% set plot_U_trial2 = prepare_plot_data(sos_trials, name='Essai maquette 2',x = 'Temps recalé (s).2', y = 'Vitesse (m/s).1')%}
{% set plot_U_trial3 = prepare_plot_data(sos_trials, name='Essai maquette 3',x = 'Temps recalé (s).4', y = 'Vitesse (m/s).2')%}
{% set graph_U     = cartesian_graph([plot_U_xdyn, plot_U_trial1, plot_U_trial2, plot_U_trial3], x='t (s)', y='$U [m/s]$', p={'grid': True}) %}
{{layout(size=(1,1), graphs=[(graph_U, (0,0))], title="Décélération")}}

## Comparaison avec le simulateur SOS-stabilité
{% set plot_U_sos_sim = prepare_plot_data(sos_trials, name='Simulateur SOS',x = 'Temps recalé (s).6', y = 'Vitesse (m/s).3')%}
{% set graph_U     = cartesian_graph([plot_U_xdyn, plot_U_sos_sim], x='t (s)', y='$U [m/s]$', p={'grid': True}) %}
{{layout(size=(1,1), graphs=[(graph_U, (0,0))], title="Décélération")}}

## Comparaison avec les essais à la mer

{% set plot_U_sos_sea1 = prepare_plot_data(sos_trials, name='Essai réel 1',x = 'Temps recalé (s).7', y = 'Vitesse (m/s).4')%}
{% set plot_U_sos_sea2 = prepare_plot_data(sos_trials, name='Essai réel 2',x = 'Temps recalé (s).8', y = 'Vitesse (m/s).5')%}
{% set graph_U     = cartesian_graph([plot_U_xdyn, plot_U_sos_sea1, plot_U_sos_sea2], x='t (s)', y='$U [m/s]$', p={'grid': True}) %}
{{layout(size=(1,1), graphs=[(graph_U, (0,0))], title="Décélération")}}

# Giration

## Scénario
{% set scenario_filename = 'reference_data_for_validation/Giration/anthineas_giration.yml' %}

## Configuration de X-DYN
Le fichier de configuration a la forme suivante :

{% set yaml_data = load(scenario_filename)%}
{{show(yaml_data)}}

{% set command_filename = 'reference_data_for_validation/Giration/controlled_forces_commands.yml' %}
On simule 50 secondes par pas de 0.2 secondes :
{{exec('xdyn '+ scenario_filename+ ' ' + command_filename + ' --dt 0.2  --tend 50 -o xdyn.h5')}}
{% set xdyn_res = h5('xdyn.h5', t='outputs/t', X='outputs/states/Anthineas/X', Y='outputs/states/Anthineas/Y', PSI='outputs/states/Anthineas/PSI', U='outputs/states/Anthineas/U', V='outputs/states/Anthineas/V', R='outputs/states/Anthineas/R') %}
{% set sos_res = xls('reference_data_for_validation/Giration/comparaison_giration_20150224.xlsx',sheetname='SOS') %}

## Comparaison des positions et attitudes dans le plan X,Y avec le simulateur SOS
{% set plot_xdyn_xy = prepare_plot_data(xdyn_res, name='xdyn', x = 'X', y = 'Y') %}
{% set plot_sos_xy  = prepare_plot_data(sos_res, name='SOS',x = 'x (m)', y = 'y (m)') %}
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

{% set graph_XY     = cartesian_graph([plot_xdyn_xy, plot_sos_xy], x='x (m)', y='y (m)', p={'grid': True, 'axis':'equal'}) %}
{{layout(size=(1,1), graphs=[(graph_XY, (0,0))], title="Position")}}

{% set plot_xdyn_tpsi = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y = 'PSI')%}
{% set plot_sos_tpsi = prepare_plot_data(sos_res, name='SOS',x = 't( s)', y = 'psi (rad)')%}
{% set graph_PSI     = cartesian_graph([plot_xdyn_tpsi, plot_sos_tpsi], x='t(s)', y='$\psi [rad]$', p={'grid': True}) %}
{{layout(size=(1,1), graphs=[(graph_PSI, (0,0))], title="Cap")}}
