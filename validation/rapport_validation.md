

{% macro set_filenames(basename) %}
{% set yaml_file = basename + '.yml' %}
{% set xdyn = basename + '.h5' %}
{% set sos = basename + '_sos.h5' %}
{% set trials = basename + '_trials.h5' %}
{% set yaml_data = load(yaml_file) %}
{% endmacro %}

{% macro plot_position_and_attitude(xdyn_res, sos_res) %}
{% set curves_for_xdyn = plot_attitude_and_displacement(xdyn_res, t='outputs/t', x='outputs/states/Anthineas/X', y='outputs/states/Anthineas/Y', z='outputs/states/Anthineas/Z', phi='outputs/states/Anthineas/PHI', theta='outputs/states/Anthineas/THETA', psi='outputs/states/Anthineas/PSI') %}
{% set curves_for_sos = plot_attitude_and_displacement(sos_res, t='outputs/t', x='outputs/states/Anthineas/X', y='outputs/states/Anthineas/Y', z='outputs/states/Anthineas/Z', phi='outputs/states/Anthineas/PHI', theta='outputs/states/Anthineas/THETA', psi='outputs/states/Anthineas/PSI') %}
{% set x_graph     = cartesian_graph([curves_for_xdyn[0],curves_for_sos[0], x='t (s)', y='$X [m]$', {'grid': 'on'}) %}
{% set y_graph     = cartesian_graph([curves_for_xdyn[1],curves_for_sos[1], x='t (s)', y='$Y [m]$', {'grid': 'on'}) %}
{%  set z_graph     = cartesian_graph([curves_for_xdyn[2],curves_for_sos[2], x='t (s)', y='$Z [m]$', {'grid': 'on'}) %}
{% set phi_graph   = cartesian_graph([curves_for_xdyn[3],curves_for_sos[3], x='t (s)', y='$\phi$ [°]', {'grid': 'on'}) %}
{% set theta_graph = cartesian_graph([curves_for_xdyn[4],curves_for_sos[4], x='t (s)', y='$\theta$ [°]', {'grid': 'on'}) %}
{% set psi_graph   = cartesian_graph([curves_for_xdyn[5],curves_for_sos[5], x='t (s)', y='$\psi$ [°]', {'grid': 'on'}) %}
{{layout((2,3), [(x_graph, (0,0)), (y_graph, (0,1)), (z_graph, (0,2)), (phi_graph, (1,0)), (theta_graph, (1,1)), (psi_graph, (1,2))])}}
{% endmacro %}


# Extinction en pilonnement
## Scénario
{% set_filenames('anthineas_amortissementPilonnement') %}


## Configuration de X-DYN
Le fichier de configuration a la forme suivante :

{{show(yaml_data)}}

On simule 50 secondes par pas de 0.2 secondes :
{{sim(yaml_file, dt=0.2, tend=50, o=xdyn)}}
{% set xdyn_res = hdf5(xdyn) %}
{% set sos_res = hdf5(sos) %}
{% set trials_res = hdf5(trials) %}


## Comparaison avec le simulateur SOS-stabilité

### Déplacements et attitude
{% plot_position_and_attitude(xdyn_res, sos_res) %}

### Vitesses locales
{% plot_speed_in_body(xdyn_res, sos_res) %}

### Vitesses NED
{% plot_speed_in_NED(xdyn_res, sos_res) %}


## Comparaison avec les essais SOS-stabilité

### Déplacements et attitude
{% plot_position_and_attitude(xdyn_res, trials_res) %}

### Vitesses locales
{% plot_speed_in_body(xdyn_res, trials_res) %}

### Vitesses NED
{% plot_speed_in_NED(xdyn_res, trials_res) %}

# Extinction en roulis
## Scénario
{% set_filenames('anthineas_amortissementRoulis') %}

## Configuration de X-DYN
Le fichier de configuration a la forme suivante :

{{show(yaml_data)}}

On simule 50 secondes par pas de 0.2 secondes :
{{sim(yaml_file, dt=0.2, tend=50, o=xdyn)}}
{% set xdyn_res = hdf5(xdyn) %}
{% set sos_res = hdf5(sos) %}
{% set trials_res = hdf5(trials) %}

## Comparaison avec le simulateur SOS-stabilité

### Déplacements et attitude
{% plot_position_and_attitude(xdyn_res, sos_res) %}

### Vitesses locales
{% plot_speed_in_body(xdyn_res, sos_res) %}

### Vitesses NED
{% plot_speed_in_NED(xdyn_res, sos_res) %}


## Comparaison avec les essais SOS-stabilité

### Déplacements et attitude
{% plot_position_and_attitude(xdyn_res, trials_res) %}

### Vitesses locales
{% plot_speed_in_body(xdyn_res, trials_res) %}

### Vitesses NED
{% plot_speed_in_NED(xdyn_res, trials_res) %}

# Extinction en tangage
## Scénario
{% set_filenames('anthineas_amortissementTangage') %}

## Configuration de X-DYN
Le fichier de configuration a la forme suivante :

{{show(yaml_data)}}

On simule 50 secondes par pas de 0.2 secondes :
{{sim(yaml_file, dt=0.2, tend=50, o=xdyn)}}
{% set xdyn_res = hdf5(xdyn) %}
{% set sos_res = hdf5(sos) %}
{% set trials_res = hdf5(trials) %}

## Comparaison avec le simulateur SOS-stabilité

### Déplacements et attitude
{% plot_position_and_attitude(xdyn_res, sos_res) %}

### Vitesses locales
{% plot_speed_in_body(xdyn_res, sos_res) %}

### Vitesses NED
{% plot_speed_in_NED(xdyn_res, sos_res) %}


## Comparaison avec les essais SOS-stabilité

### Déplacements et attitude
{% plot_position_and_attitude(xdyn_res, trials_res) %}

### Vitesses locales
{% plot_speed_in_body(xdyn_res, trials_res) %}

### Vitesses NED
{% plot_speed_in_NED(xdyn_res, trials_res) %}





# Accélération

# Décélération
## Scénario
## Configuration de X-DYN
## Comparaison avec le simulateur SOS-stabilité
## Comparaison avec les essais SOS-stabilité

# Comparaisons avec DYSCO et DYSCO-ASM

## Barres en croix
### Scénario
### Configuration de X-DYN
### Résultats

## Barres en X
### Scénario
### Configuration de X-DYN
### Résultats
