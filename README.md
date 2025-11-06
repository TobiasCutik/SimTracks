# SimTracks

SimTracks enables users to effectively steer, evaluate
and compare different simulation runs with different
parameters over the course of the whole simulation time.
It provides a timeline with various branches, representing
different simulation runs.

Made with Unreal Engine 5.5

## Showcase
https://youtu.be/IanIqjfQgKI

## How to include own simulations

In this repository, there is a sample epedemic simulation used, but you can include also any other simulation.

It is recommended to provide a deterministic simulation, otherwise there may occur inconsistencies within simulation runs and when evaluating their performances.
Also, it should not depend on Unreal Engine's ``Tick()``, as it is frame rate dependent and therefore somewhat inconsistent.


### SimTracks Classes to inherit from
#### UserSimulation Class 
This class is responsible to setup and advance the simulation. Also simulation and evaluation parameters are setup and utilized here.

##### Parameter Setup 
Users are responsible to setup the simulation parameters and evaluation parameters. This setup may be done in ``OnConstruction()`` which gets directly called after the creation of ``UUserSimulation``. The parameter setup should not be done directly in the constructor. Also not in the provided ``Initialize()``, as the simulation and evaluation parameters are needed by other classes before that. 

##### Initialization 
The ``Initialize()`` is called when the simulation is added to ``ASimulator``. Here, the part of the simulation should be setup that may rely upon objects in the level. The mentioned data 
can be passed via a blueprint child class of ``ASimSetup``. 

##### Applying new SimStates 
Whenever the current simulation time gets changed by the time cursor and also upon branching, the ``ApplySimState()`` gets called. With it, the new ``FSimState`` is passed and users should apply the data of the new state to their simulation. 

##### Advancing the Simulation 
In ``Simulate()`` the user-provided simulation gets advanced one time step further. As there exists ``ApplySimState()`` there is no need to completely apply the passed ``FSimState`` to the simulation. It is sufficient to use the state’s values to calculate the next simulation step. At the end of ``Simulate()``, all data from the simulation should be applied to the ``FSimState`` as it will be reused in the next simulation step and therefore needs the most recent data. 

##### Calculating Evaluation Parameters 
As users provide their own evaluation parameters, they also need to calculate and update the values for them. This should be done at the end of ``Simulate()``. 

#### SimSetup Class 
``ASimSetup`` is used to setup the whole simulation system, i.e. it connects all relevant classes used in SimTracks. Users must inherit from this class via a blueprint child. Also, they need to provide the user interface, the inherited ``UUserSimulation`` and the ``BP_Simulator`` that needs to be in the level. 
As ``ASimSetup`` is an ``AActor``, users can add there any objects, collisions etc. that are used by the simulation in the level. This class is the only way to pass data to the user’s simulation class. If users need to do that, they can do so via Blueprint code. For that, the Blueprint event ``Initialize()`` is provided. Within this event, the provided ``UUserSimulation`` is already constructed and can be accessed via the protected ``UserSimulation`` property. 

#### User Interface 
The used UI class needs to be set in ``ASimSetup``. There, a ``USimTracksUIWrapper`` is expected, which requires to have a ``USimTracksUI`` widget, that consists of the actual base UI for SimTracks. If you need to modify the UI, you can create a new child of ``USimTracksUIWrapper``. There, you need to add a ``USimTracksUI`` widget and any additional widgets that are desired.

### Minimal Level Setup 
• The user-created ``ASimSetup``. This class initializes the whole simulation system of SimTracks and is needed to pass objects, that are present in the level, to the user simulation. 

• ``BP_Simulator``.

• ``BP_SceneCapture``. Its captured view is projected onto the user interface. 
