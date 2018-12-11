# Agent Based Crowd Sim

[Using my TAA demo as boilerplate renderer.](https://github.com/s4907224/TAA "TAA Demo")
Keeping with my tradition of ridiculous acronyms, BEAN is the title of my work-in-progress agent based crowd sim.  The concept behind BEAN is to be able to run path finding for thousands of individual and grouped entities in a large scale game environment in real time.  Inspired by Frontier Development’s talk at 2018’s BFX festival, I plan on utilising pre-computed flow fields for a given environment to allow potentially thousands of agents to navigate to and from any point in the environment.  The main shakeup is to subdivide the environment into smaller sections to save on data, while attempting to maintain path accuracy.
