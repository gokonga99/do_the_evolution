#include "Agent.hpp"

Agent::Agent(int height, int width, std::string &weightsFile)
{
    age = 0;
    genome.size = 1 + rand() % 3;
    genome.perception = 1 + rand() % 2;
    genome.moveCooldown = 1 + rand() % 3;
    genome.fertility = 1 + rand() % 3;
    maxAge = 300 + genome.moveCooldown * 100;
    matingCooldown = 40;
    maxEnergy = 120.0f * genome.size;
    energy = maxEnergy;

    // genome.hasNN = (rand() % 1 == 0);
    genome.hasNN=false;
    if (genome.hasNN)
    {
        std::ifstream file(weightsFile);

        if (!file.is_open())
        {
            std::cout << "FAILED TO OPEN: " << weightsFile << std::endl;
        }
        size_t weightCount;
        file >> weightCount;

        genome.nnWeights.resize(weightCount);

        for (size_t j = 0; j < weightCount; j++)
            file >> genome.nnWeights[j];

        std::cout << "Loaded weight count: "
                  << weightCount
                  << std::endl;
        // genome.nnWeights.resize(NeuralNetwork::TOTAL_WEIGHTS);
        // for (auto &w : genome.nnWeights)
        // {
        //     w += ((rand() % 200) - 100) / 100.0f;
        //     w = std::max(-1.0f, std::min(1.0f, w));
        // }
    }
    state = AgentState::Alive;
    dyingTimer = 0;
    x = rand() % width;
    y = rand() % height;
    readyToMate = false;
}

Agent::Agent(Genome &genome, int parentX, int parentY, int height, int width)
{
    age = 0;
    this->genome = genome;
    maxAge = 300 + genome.moveCooldown * 50;
    x = parentX;
    y = parentY;
    matingCooldown = 20 * genome.fertility;
    maxEnergy = 120.0f * genome.size;
    energy = 60.0f;

    state = AgentState::Alive;
    dyingTimer = 0;
    readyToMate = false;
}

int Agent::getX() const
{
    return x;
}
int Agent::getY() const
{
    return y;
}
int Agent::getSize() const
{
    return genome.size;
}
AgentState Agent::getState() const
{
    return state;
}
Genome Agent::getGenome() const
{
    return genome;
}
int Agent::getPerception() const
{
    return genome.perception;
}
bool Agent::isReadyToMate()
{
    return readyToMate;
}
int Agent::getAge() const
{
    return age;
}

void Agent::mate()
{

    matingCooldown = 30;
    energy -= 30.0f;
    readyToMate = false;
    if (energy < 0)
        energy = 0;
}
void Agent ::eat(Environment &env)
{
    if (env.getFoodType(x, y) == 1)
    {
        env.consumeFood(x, y);
        energy += 15.0f;
        if (energy > maxEnergy)
            energy = maxEnergy;
    }
    else if (env.getFoodType(x, y) == 2 && genome.size > 1)
    {
        env.consumeFood(x, y);
        energy += 30.0f * (float)genome.size;

        if (energy > maxEnergy)
            energy = maxEnergy;
    }
}
void Agent::moveTo(int newX, int newY, Environment &env)
{
    if (env.isValidPosition(newX, newY))
    {

        int tempX = x;
        int tempY = y;
        if (x < newX)
            tempX++;
        else if (x > newX)
            tempX--;
        if (y < newY)
            tempY++;
        else if (y > newY)
            tempY--;
        energy -= (float)genome.size;

        if (env.isValidPosition(tempX, tempY))
        {
            x = tempX;
            y = tempY;
        }
    }
    eat(env);
}
void Agent ::randomWalk(Environment &env)
{
    int rdx = (rand() % 3) - 1;
    int rdy = (rand() % 3) - 1;
    if (env.isValidPosition(x + rdx, y + rdy))
    {
        x += rdx;
        y += rdy;
        if (rdx != 0 || rdy != 0)
            energy -= (float)genome.size;
        eat(env);
    }
}
ScanResult Agent::scan(Environment &env)
{
    energy -= 0.5f * (float)genome.perception;
    int tempFoodX = x + genome.perception;
    int tempFoodY = y + genome.perception;
    int tempRareFoodX = x + genome.perception;
    int tempRareFoodY = y + genome.perception;

    int startX, endX, stepX;
    int startY, endY, stepY;
    ScanResult result;

    if (rand() % 2)
    {
        startX = x - genome.perception;
        endX = x + genome.perception;
        stepX = 1;
    }
    else
    {
        startX = x + genome.perception;
        endX = x - genome.perception;
        stepX = -1;
    }

    if (rand() % 2)
    {
        startY = y - genome.perception;
        endY = y + genome.perception;
        stepY = 1;
    }
    else
    {
        startY = y + genome.perception;
        endY = y - genome.perception;
        stepY = -1;
    }

    for (int i = startX; i != endX + stepX; i += stepX)
    {
        for (int j = startY; j != endY + stepY; j += stepY)
        {
            if ((env.getFoodType(i, j) == 2 && genome.size > 1) ||
                (env.getFoodType(i, j) == 1))
            {
                int distance = std::max(abs(i - x), abs(j - y));
                int distanceRare = std::max(abs(i - x), abs(j - y));
                result.localFoodCount++;
                if (result.distanceToFood > distance)
                {
                    tempFoodX = i;
                    tempFoodY = j;
                    result.distanceToFood = distance;
                    result.foodVisible = true;
                }
                // used only by NN
                if (genome.size > 1 && env.getFoodType(i, j) == 2 && result.distanceToRareFood > distanceRare)
                {
                    tempRareFoodX = i;
                    tempRareFoodY = j;
                    result.distanceToRareFood = distance;
                    result.rareFoodVisible = true;
                }
            }
        }
    }
    if (result.foodVisible)
    {
        result.foodX = tempFoodX;
        result.foodY = tempFoodY;
    }
    if (result.rareFoodVisible)
    {
        result.rareFoodX = tempRareFoodX;
        result.rareFoodY = tempRareFoodY;
    }
    result.localFoodCount /= (float)((2 * genome.perception + 1) * (2 * genome.perception + 1) - 1);

    return result;
}

void Agent::update(Environment &env, int mateX, int mateY, int mateDistance)
{
    age++;
    if (state == AgentState::Dying)
    {
        dyingTimer++;
        if (dyingTimer >= 3)
        {
            state = AgentState::Dead;
            env.getStats().deaths++;
        }
        return;
    }
    if (age > maxAge || energy <= 0)
    {
        state = AgentState::Dying;
        return;
    }
    if (age <= maxAge && energy > 0)
    {

        energy -= 1.0f / (float)genome.moveCooldown;
        bool mateVisable = (mateX != -1);

        if (energy < maxEnergy / 2 || matingCooldown != 0)
            readyToMate = false;
        else if (energy >= maxEnergy / 2 && matingCooldown == 0)
            readyToMate = true;

        if (matingCooldown > 0)
            matingCooldown--;

        ScanResult result = scan(env);

        if (age % genome.moveCooldown == 0)
        {
            if (genome.hasNN && genome.nnWeights.size() == NeuralNetwork::TOTAL_WEIGHTS)
            {
                energy -= 0.5f;
                nn.loadWeights(genome.nnWeights);

                std::vector<float> inputs = {
                    energy / maxEnergy,
                    result.foodVisible ? 1.0f : 0.0f,
                    result.foodVisible ? result.distanceToFood / (float)genome.perception : -1.0f,
                    result.rareFoodVisible ? 1.0f : 0.0f,
                    result.rareFoodVisible ? result.distanceToRareFood / (float)genome.perception : -1.0f,
                    mateVisable ? 1.0f : 0.0f,
                    mateVisable ? mateDistance / (float)genome.perception : -1.0f,
                    readyToMate ? 1.0f : 0.0f,
                    result.foodVisible ? result.localFoodCount : 0.0f,
                };
                int action = nn.compute(inputs);
                env.getStats().totalDecisions++;
                int decision = 1;

                switch (action)
                {
                case 0:
                    if (result.foodVisible)
                    {
                        env.getStats().foodDecisions++;
                        moveTo(result.foodX, result.foodY, env);
                    }
                    else
                    {
                        env.getStats().foodImpossible++;
                        
                    }
                    break;
                case 1:
                    if (result.rareFoodVisible)
                    {
                        env.getStats().rareFoodDecisions++;
                        moveTo(result.rareFoodX, result.rareFoodY, env);
                    }
                    else
                    {
                        env.getStats().rareImpossible++;
                    }
                    break;
                case 2:
                    if (mateVisable && readyToMate)
                    {
                        env.getStats().mateDecisions++;
                        moveTo(mateX, mateY, env);
                    }
                    else
                    {
                        env.getStats().mateImpossible++;
                        
                    }
                    break;
                case 3:
                    env.getStats().wanderDecisions++;
                    randomWalk(env);
                    break;
                case 4:
                    env.getStats().stayDecisions++;
                    eat(env);
                    break;
                }
            }
            else
            {
                if (result.foodVisible)
                    moveTo(result.foodX, result.foodY, env);
                else if (mateVisable && readyToMate)
                    moveTo(mateX, mateY, env);
                else
                {
                    randomWalk(env);
                }
            }
        }
    }
}
