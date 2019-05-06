```python echo=False, results='hidden'
def plot_position_and_attitude(xdyn_res, sos_res):
    plot_x_xdyn = prepare_plot_data(xdyn_res, name='xdyn',x = 't', y='x')
    plot_y_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='y')
    plot_z_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='z')
    plot_phi_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='phi')
    plot_theta_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='theta')
    plot_psi_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='psi')
    plot_x_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y='x (m)')
    plot_y_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y='y (m)')
    plot_z_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y='z (m)')
    plot_phi_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y='phi (rad)')
    plot_theta_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y='theta (rad)')
    plot_psi_sos = prepare_plot_data(sos_res, name='sos', x = 't( s)', y='psi (rad)')
    x_graph     = cartesian_graph([plot_x_xdyn, plot_x_sos], x='t (s)', y='$X [m]$', p={'grid': True})
    y_graph     = cartesian_graph([plot_y_xdyn, plot_y_sos], x='t (s)', y='$Y [m]$', p={'grid': True})
    z_graph     = cartesian_graph([plot_z_xdyn, plot_z_sos], x='t (s)', y='$Z [m]$', p={'grid': True})
    phi_graph   = cartesian_graph([plot_phi_xdyn, plot_phi_sos], x='t (s)', y='$\phi$ [rad]', p={'grid': True})
    theta_graph = cartesian_graph([plot_theta_xdyn, plot_theta_sos], x='t(s)', y='$\\theta$ [rad]', p={'grid': True})
    psi_graph   = cartesian_graph([plot_psi_xdyn, plot_psi_sos], x='t (s)', y='$\psi$ [rad]', p={'grid': True})
    layout(size=(2,3), graphs=[(x_graph, (0,0)), (y_graph, (0,1)), (z_graph, (0,2)), (phi_graph, (1,0)), (theta_graph, (1,1)), (psi_graph, (1,2))], title="Attitude navire")

def plot_speed(xdyn_res, sos_res):
    plot_U_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='U')
    plot_V_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='V')
    plot_W_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='W')
    plot_P_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='P')
    plot_Q_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='Q')
    plot_R_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x='t', y='R')
    plot_U_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='u (m/s)')
    plot_V_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='v (m/s)')
    plot_W_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='w (m/s)')
    plot_P_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='p (rad/s)')
    plot_Q_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='q (rad/s)')
    plot_R_sos = prepare_plot_data(sos_res, name='sos', x='t( s)', y='r (rad/s)')
    U_graph = cartesian_graph([plot_U_xdyn, plot_U_sos], x='t (s)', y='$U [m/s]$', p={'grid': True})
    V_graph = cartesian_graph([plot_V_xdyn, plot_V_sos], x='t (s)', y='$V [m/s]$', p={'grid': True})
    W_graph = cartesian_graph([plot_W_xdyn, plot_W_sos], x='t (s)', y='$W [m/s]$', p={'grid': True})
    P_graph = cartesian_graph([plot_P_xdyn, plot_P_sos], x='t (s)', y='$P$ [rad/s]', p={'grid': True})
    Q_graph = cartesian_graph([plot_Q_xdyn, plot_Q_sos], x='t (s)', y='$Q$ [rad/s]', p={'grid': True})
    R_graph = cartesian_graph([plot_R_xdyn, plot_R_sos], x='t (s)', y='$R$ [rad/s]', p={'grid': True})
    layout(size=(2,3), graphs=[(U_graph, (0,0)), (V_graph, (0,1)), (W_graph, (0,2)), (P_graph, (1,0)), (Q_graph, (1,1)), (R_graph, (1,2))], title="Vitesses")
```

# Extinction en pilonnement

## Scénario

```python echo=False, results='hidden'
scenario_filename = 'reference_data_for_validation/Extinctions_simu/test_ship_amortissementPilonnement_sans_controleur.yml'
```

## Configuration de X-DYN

Le fichier de configuration a la forme suivante :

```python echo=False, results='hidden'
yaml_data = load(scenario_filename)
show(yaml_data)
```

On simule 50 secondes par pas de 0.2 seconde :

```python echo=False, results='raw'
execCommand('xdyn ' + scenario_filename + ' --dt 0.2 --tend 50 -o xdyn.h5')
```

## Comparaison avec le simulateur SOS-stabilité

### Déplacements et attitude

```python echo=False, results='raw'
xdyn_res = h5('xdyn.h5', t='outputs/t',
                         x='outputs/states/Anthineas/X',
                         y='outputs/states/Anthineas/Y',
                         z='outputs/states/Anthineas/Z',
                         phi='outputs/states/Anthineas/PHI',
                         theta='outputs/states/Anthineas/THETA',
                         psi='outputs/states/Anthineas/PSI',
                         U='outputs/states/Anthineas/U',
                         V='outputs/states/Anthineas/V',
                         W='outputs/states/Anthineas/W',
                         P='outputs/states/Anthineas/P',
                         Q='outputs/states/Anthineas/Q',
                         R='outputs/states/Anthineas/R')
sos_res = xls('reference_data_for_validation/Extinctions_simu/comparaison_extinctionPilonnement_zIni_2_simulateur_SOS.xlsx', sheetname='SOS')
plot_position_and_attitude(xdyn_res, sos_res)
```

### Vitesses

```python echo=False, results='raw'
plot_speed(xdyn_res, sos_res)
```

# Extinction en roulis

## Scénario

```python echo=False, results='raw'
scenario_filename = 'reference_data_for_validation/Extinctions_simu/test_ship_amortissementRoulis_sans_controleur.yml'
```

## Configuration de X-DYN

Le fichier de configuration a la forme suivante :


```python echo=False, results='raw'
yaml_data = load(scenario_filename)
show(yaml_data)
```

On simule 50 secondes par pas de 0.2 seconde :

```python echo=False, results='raw'
execCommand('xdyn '+scenario_filename + ' --dt 0.2 --tend 50 -o xdyn.h5')
xdyn_res = h5('xdyn.h5', t='outputs/t',
                         x='outputs/states/Anthineas/X',
                         y='outputs/states/Anthineas/Y',
                         z='outputs/states/Anthineas/Z',
                         phi='outputs/states/Anthineas/PHI',
                         theta='outputs/states/Anthineas/THETA',
                         psi='outputs/states/Anthineas/PSI',
                         U='outputs/states/Anthineas/U',
                         V='outputs/states/Anthineas/V',
                         W='outputs/states/Anthineas/W',
                         P='outputs/states/Anthineas/P',
                         Q='outputs/states/Anthineas/Q',
                         R='outputs/states/Anthineas/R')
sos_res = xls('reference_data_for_validation/Extinctions_simu/comparaison_extinctionRoulis_phiIni_10_simulateur_SOS.xlsx', sheetname='SOS')
```

## Comparaison avec le simulateur SOS-stabilité

### Déplacements et attitude

```python echo=False, results='raw'
plot_position_and_attitude(xdyn_res, sos_res)
```

### Vitesses

```python echo=False, results='raw'
plot_speed(xdyn_res, sos_res)
```

# Extinction en tangage

## Scénario

```python echo=False, results='raw'
scenario_filename = 'reference_data_for_validation/Extinctions_simu/test_ship_amortissementTangage_sans_controleur.yml'
```


## Configuration de X-DYN

Le fichier de configuration a la forme suivante :

```python echo=False, results='raw'
yaml_data = load(scenario_filename)
show(yaml_data)
```

On simule 50 secondes par pas de 0.2 seconde :

```python echo=False, results='raw'
execCommand('xdyn '+ scenario_filename+ ' --dt 0.2  --tend 50 -o xdyn.h5')
xdyn_res = h5('xdyn.h5', t='outputs/t',
                         x='outputs/states/Anthineas/X',
                         y='outputs/states/Anthineas/Y',
                         z='outputs/states/Anthineas/Z',
                         phi='outputs/states/Anthineas/PHI',
                         theta='outputs/states/Anthineas/THETA',
                         psi='outputs/states/Anthineas/PSI',
                         U='outputs/states/Anthineas/U',
                         V='outputs/states/Anthineas/V',
                         W='outputs/states/Anthineas/W',
                         P='outputs/states/Anthineas/P',
                         Q='outputs/states/Anthineas/Q',
                         R='outputs/states/Anthineas/R')
sos_res = xls('reference_data_for_validation/Extinctions_simu/comparaison_extinctionTangage_thetaIni_10_simulateur_SOS.xlsx', sheetname='SOS')
```

## Comparaison avec le simulateur SOS-stabilité

### Déplacements et attitude

```python echo=False, results='raw'
plot_position_and_attitude(xdyn_res, sos_res)
```

### Vitesses

```python echo=False, results='raw'
plot_speed(xdyn_res, sos_res)
```

# Accélération

## Scénario

```python echo=False, results='raw'
scenario_filename = 'reference_data_for_validation/AccelerationDeceleration/test_ship_acceleration.yml'
```

## Configuration de X-DYN

Le fichier de configuration a la forme suivante :

```python echo=False, results='raw'
yaml_data = load(scenario_filename)
show(yaml_data)
```

On simule 50 secondes par pas de 0.2 seconde :

```python echo=False, results='raw'
execCommand('xdyn '+ scenario_filename+ ' --dt 0.2  --tend 50 -o xdyn.h5')
xdyn_res = h5('xdyn.h5', t='outputs/t', U='outputs/states/Anthineas/U')
sos_trials = xls('reference_data_for_validation/AccelerationDeceleration/AnalyseEssaisErdre_201212.xlsx', sheetname='Accélération', skiprows=3)
```

## Comparaison avec les essais SOS-stabilité

```python echo=False, results='raw'
plot_U_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='U')
plot_U_trial1 = prepare_plot_data(sos_trials, name='essais 1',x = 'Temps recalé (s)', y='Vitesse (m/s)')
plot_U_trial2 = prepare_plot_data(sos_trials, name='essais 2',x = 'Temps recalé (s).1', y='Vitesse (m/s).1')
plot_U_trial3 = prepare_plot_data(sos_trials, name='essais 3',x = 'Temps recalé (s).2', y='Vitesse (m/s).2')
graph_U       = cartesian_graph([plot_U_xdyn, plot_U_trial1, plot_U_trial2, plot_U_trial3], x='t (s)', y='$U [m/s]$', p={'grid': True})
layout(size=(1,1), graphs=[(graph_U, (0,0))], title="Accélération")
```

## Comparaison avec le simulateur SOS-stabilité

```python echo=False, results='raw'
plot_U_sos_sim = prepare_plot_data(sos_trials, name='simulateur SOS',x = 'Temps recalé (s).3', y='Vitesse (m/s).3')
graph_U     = cartesian_graph([plot_U_xdyn, plot_U_sos_sim], x='t (s)', y='$U [m/s]$', p={'grid': True})
layout(size=(1,1), graphs=[(graph_U, (0,0))], title="Accélération")
```

# Décélération

## Scénario

```python echo=False, results='raw'
scenario_filename = 'reference_data_for_validation/AccelerationDeceleration/test_ship_deceleration.yml'
```

## Configuration de X-DYN
Le fichier de configuration a la forme suivante :

```python echo=False, results='raw'
yaml_data = load(scenario_filename)
show(yaml_data)
```

On simule 50 secondes par pas de 0.2 seconde :

```python echo=False, results='raw'
execCommand('xdyn '+ scenario_filename+ ' --dt 0.2  --tend 50 -o xdyn.h5')
xdyn_res = h5('xdyn.h5', t='outputs/t', U='outputs/states/Anthineas/U')
sos_trials = xls('reference_data_for_validation/AccelerationDeceleration/AnalyseEssaisErdre_201212.xlsx', sheetname='Décélération', skiprows=3)
plot_U_xdyn = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='U')
```

## Comparaison avec les essais maquette SOS-stabilité échelle 1/11e

```python echo=False, results='raw'
plot_U_trial1 = prepare_plot_data(sos_trials, name='Essai maquette 1',x = 'Temps recalé (s)', y='Vitesse (m/s)')
plot_U_trial2 = prepare_plot_data(sos_trials, name='Essai maquette 2',x = 'Temps recalé (s).2', y='Vitesse (m/s).1')
plot_U_trial3 = prepare_plot_data(sos_trials, name='Essai maquette 3',x = 'Temps recalé (s).4', y='Vitesse (m/s).2')
graph_U       = cartesian_graph([plot_U_xdyn, plot_U_trial1, plot_U_trial2, plot_U_trial3], x='t (s)', y='$U [m/s]$', p={'grid': True})
layout(size=(1,1), graphs=[(graph_U, (0,0))], title="Décélération")
```

## Comparaison avec le simulateur SOS-stabilité

```python echo=False, results='raw'
plot_U_sos_sim = prepare_plot_data(sos_trials, name='Simulateur SOS',x = 'Temps recalé (s).6', y='Vitesse (m/s).3')
graph_U     = cartesian_graph([plot_U_xdyn, plot_U_sos_sim], x='t (s)', y='$U [m/s]$', p={'grid': True})
layout(size=(1,1), graphs=[(graph_U, (0,0))], title="Décélération")
```

## Comparaison avec les essais à la mer

```python echo=False, results='raw'
plot_U_sos_sea1 = prepare_plot_data(sos_trials, name='Essai réel 1',x = 'Temps recalé (s).7', y='Vitesse (m/s).4')
plot_U_sos_sea2 = prepare_plot_data(sos_trials, name='Essai réel 2',x = 'Temps recalé (s).8', y='Vitesse (m/s).5')
graph_U     = cartesian_graph([plot_U_xdyn, plot_U_sos_sea1, plot_U_sos_sea2], x='t (s)', y='$U [m/s]$', p={'grid': True})
layout(size=(1,1), graphs=[(graph_U, (0,0))], title="Décélération")
```

# Giration

## Scénario

```python echo=False, results='raw'
scenario_filename = 'reference_data_for_validation/Giration/test_ship_giration.yml'
```

## Configuration de X-DYN

Le fichier de configuration a la forme suivante :


```python echo=False, results='raw'
yaml_data = load(scenario_filename)
show(yaml_data)
```

```python echo=False, results='raw'
command_filename = 'reference_data_for_validation/Giration/controlled_forces_commands.yml'
```

On simule 50 secondes par pas de 0.2 seconde :

```python echo=False, results='raw'
execCommand('xdyn '+ scenario_filename+ ' ' + command_filename + ' --dt 0.2  --tend 50 -o xdyn.h5')
xdyn_res = h5('xdyn.h5', t='outputs/t', X='outputs/states/Anthineas/X', Y='outputs/states/Anthineas/Y', PSI='outputs/states/Anthineas/PSI', U='outputs/states/Anthineas/U', V='outputs/states/Anthineas/V', R='outputs/states/Anthineas/R')
sos_res = xls('reference_data_for_validation/Giration/comparaison_giration_20150224.xlsx',sheetname='SOS')
```

## Comparaison des positions et attitudes dans le plan X,Y avec le simulateur SOS


```python echo=False, results='raw'
plot_xdyn_xy = prepare_plot_data(xdyn_res, name='xdyn', x = 'X', y='Y')
plot_sos_xy  = prepare_plot_data(sos_res, name='SOS',x = 'x (m)', y='y (m)')
graph_XY     = cartesian_graph([plot_xdyn_xy, plot_sos_xy], x='x (m)', y='y (m)', p={'grid': True, 'axis':'equal'})
layout(size=(1,1), graphs=[(graph_XY, (0,0))], title="Position")
```

```python echo=False, results='raw'
plot_xdyn_tpsi = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='PSI')
plot_sos_tpsi = prepare_plot_data(sos_res, name='SOS',x = 't( s)', y='psi (rad)')
graph_PSI     = cartesian_graph([plot_xdyn_tpsi, plot_sos_tpsi], x='t(s)', y='$\psi [rad]$', p={'grid': True})
layout(size=(1,1), graphs=[(graph_PSI, (0,0))], title="Cap")
```

## Comparaison des vitesses dans le plan X,Y avec le simulateur SOS

```python echo=False, results='raw'
plot_xdyn_u = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='U')
plot_sos_u = prepare_plot_data(sos_res, name='SOS',x = 't( s)', y='u (m/s)')
graph_U     = cartesian_graph([plot_xdyn_u, plot_sos_u], x='t (s)', y='U (m/s)', p={'grid': True})
```

```python echo=False, results='raw'
plot_xdyn_v = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='V')
plot_sos_v = prepare_plot_data(sos_res, name='SOS',x = 't( s)', y='v (m/s)')
graph_V     = cartesian_graph([plot_xdyn_v, plot_sos_v], x='t (s)', y='V (m/s)', p={'grid': True})
```

```python echo=False, results='raw'
plot_xdyn_r = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='R')
plot_sos_r = prepare_plot_data(sos_res, name='SOS',x = 't( s)', y='r (rad/s)')
graph_R     = cartesian_graph([plot_xdyn_r, plot_sos_r], x='t (s)', y='R (rad/s)', p={'grid': True})
layout(size=(1,3), graphs=[(graph_U, (0,0)), (graph_V, (0,1)), (graph_R,(0,2))], title="Vitesses")
```

```python echo=False, results='raw'
graph_XY     = cartesian_graph([plot_xdyn_xy, plot_sos_xy], x='x (m)', y='y (m)', p={'grid': True, 'axis':'equal'})
layout(size=(1,1), graphs=[(graph_XY, (0,0))], title="Position")
```

```python echo=False, results='raw'
plot_xdyn_tpsi = prepare_plot_data(xdyn_res, name='xdyn', x = 't', y='PSI')
plot_sos_tpsi = prepare_plot_data(sos_res, name='SOS',x = 't( s)', y='psi (rad)')
graph_PSI     = cartesian_graph([plot_xdyn_tpsi, plot_sos_tpsi], x='t(s)', y='$\psi [rad]$', p={'grid': True})
layout(size=(1,1), graphs=[(graph_PSI, (0,0))], title="Cap")
```
