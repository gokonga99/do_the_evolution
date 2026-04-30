#include "Simulation.hpp"
#include <algorithm>
#include <iostream>


Simulation::Simulation(int numAgents,int height, int width)
:currentTick(0),env(height,width),numAgents(numAgents)
{
    agents.reserve(numAgents);

    for(int i=0 ;i<numAgents; i++) {        
        agents.emplace_back(height, width);
    }
}

void Simulation::run(int maxTicks){

    
    for(int i=0; i<maxTicks; i++){
        tick();
    }

}

void Simulation::tick(){
    currentTick++;

    env.update();

    for(auto& agent : agents){
        agent.update(env);

        }
    agents.erase(
    std::remove_if(agents.begin(), agents.end(), [](const Agent& a){
        return a.getState() == AgentState::Dead;
    }),
    agents.end()
    );


    //clean up agents

    std::cout<<"Tick: "<<currentTick
             <<"| Agents: "<<agents.size()
             <<std::endl;
}
const Environment& Simulation::getEnv() const{
    return env;
}

const std::vector <Agent>&  Simulation :: getAgents() const{
    return agents;
}