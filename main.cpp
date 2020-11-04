#include<iostream>
using namespace std;

/*
In this project, object-oriented programming was used to create a class for a Robot.
The Robot has a fixed path and can only move either forward or backward in this path.
An initial state of the Robot include:
    - initial battery
    - initial position
    - cost per move (fixed) = -1% from the battery.
The Robot is able to move as long as the battery is larger than 0%.
The Robot waits for the input from the user to decide the next movement. 
The Robot can't move backward at the first position in the path due to boundary.
The Robot can't move foreward at the last position in the path due to the boundary.
The Robot consumes a fixed amount of energy for each movement regardless of the distance.

Further Developments: 
- Calculate the distance for each move.
- Calculate the cost to be function of the distance (e.g. 1% for each meter)
- Build an environment class with 4 direction (up, down, left, rifht).
- User reinforecment learning to find the shortest path to the final goal in a 2D.
*/

// Robot Class Header:
class Robot
{
private: 
    double battery; 
    int position; // point number in the path
    double location[2]; // x,y values in the path
    double batteryCost;
    string action;
public: 
    // Constructor: 
    Robot(double currentBattery, int currentPosition, double batteryCost);
    // Setters:
    void setBattery(double currentBattery);
    void setPosition(int currentPosition);
    void setCost(double currentCost);
    void setLocation(int currentPosition);
    void print(); 
    void forward();
    void backward();
    // Getters: 
    double getBattery();
    int getPosition();
    double getDistance(int currentPosition); 
    double getMoveCost(double distance);
    double getLocation();
};

// Path
double path[][2] = {
      {0.00359, -0.0013},   {0.00608, -0.00281},  {0.00756, -0.0027},
      {0.00842, -0.00307},  {0.00849, -0.0037},   {0.00846, -0.00387},
      {0.00829, -0.00379},  {0.0084, -0.00388},   {0.00846, -0.00409},
      {0.0138, -0.00347},   {0.0698, -0.00098},   {0.11151, -0.00745},
      {0.167, -0.01404},    {0.32572, -0.05356},  {0.41797, -0.07953},
      {0.52867, -0.11505},  {0.61002, -0.13945},  {0.63633, -0.14954},
      {0.70933, -0.18835},  {0.7191, -0.19822},   {0.72701, -0.20117},
      {0.731, -0.20424},    {0.73371, -0.20805},  {0.77746, -0.2621},
      {0.86029, -0.34734},  {0.88373, -0.37565},  {0.90413, -0.40655},
      {0.92189, -0.43795},  {0.93867, -0.47125},  {0.95337, -0.50479},
      {0.96615, -0.54003},  {0.97729, -0.57518},  {0.98669, -0.60948},
      {0.9944, -0.64442},   {0.99963, -0.67999},  {1.00244, -0.71709},
      {1.00327, -0.75302},  {0.99907, -0.78939},  {0.99464, -0.8237},
      {0.98722, -0.86223},  {0.97558, -0.90511},  {0.96147, -0.94947},
      {0.94402, -0.99336},  {0.92286, -1.03964},  {0.89779, -1.08594},
      {0.8698, -1.13096},   {0.73009, -1.33175},  {0.59053, -1.5304},
      {0.46166, -1.7128},   {0.30239, -1.93285},  {0.25147, -1.99011},
      {0.19826, -2.04254},  {0.14275, -2.09163},  {0.08663, -2.13425},
      {0.03116, -2.17209},  {-0.0238, -2.20402},  {-0.07864, -2.23286},
      {-0.1318, -2.25636},  {-0.1825, -2.27552},  {-0.23171, -2.29113},
      {-0.27795, -2.30206}, {-0.32673, -2.31054}, {-0.37225, -2.31536},
      {-0.41574, -2.31996}, {-0.45496, -2.32042}, {-0.48902, -2.31757},
      {-0.52496, -2.3164},  {-0.55811, -2.31102}, {-0.77049, -2.25292},
      {-0.99, -2.19669},    {-1.19266, -2.14085}, {-1.23428, -2.12438},
      {-1.27377, -2.10614}, {-1.31327, -2.08351}, {-1.39679, -2.03016},
      {-1.48345, -1.95929}, {-1.52353, -1.91628}, {-1.66757, -1.77012},
      {-1.83468, -1.60606}, {-2.01648, -1.41688}, {-2.18845, -1.20596},
      {-2.35403, -0.99207}, {-2.44666, -0.84068}, {-2.48383, -0.76261},
      {-2.51504, -0.68854}, {-2.53995, -0.61543}, {-2.56026, -0.54313},
      {-2.57583, -0.47095}, {-2.58632, -0.40214}, {-2.5929, -0.33388},
      {-2.59584, -0.2669},  {-2.5965, -0.20323},  {-2.59088, -0.13817},
      {-2.58415, -0.07689}, {-2.57404, -0.0163},  {-2.55813, 0.04199},
      {-2.5374, 0.10109},   {-2.51245, 0.15825},  {-2.48738, 0.21222},
      {-2.45803, 0.26488},  {-2.42471, 0.314},    {-2.38647, 0.36297},
      {-2.3471, 0.40819},   {-2.30357, 0.45124},  {-2.1598, 0.59651},
      {-1.99623, 0.75884},  {-1.84116, 0.91525},  {-1.68546, 1.07255},
      {-1.57778, 1.17373}};
int pathLength = sizeof(path)/sizeof(path[0]);

int main()
{   
    // Variables:
    double initBattery = 1; // fully charged (100%)
    int initPosition = 1; // min=0, and max=108
    double costPerMove = 0.01; // assumption: battery reduces by 1% for each movement. 
    int movement;

    // Robot: 
    Robot myRobot(initBattery, initPosition, costPerMove);

    // Start Movement according to the path:
    while (myRobot.getBattery() > 0)
    {
        cout << "-1: backward | 0: stay | 1: forward | 2: exit" << endl;
        cin >> movement;
        
        if (movement == 2)
            exit(0);
        
        switch (movement)
        {
            case -1:
                myRobot.backward();
                // Assumption: constant battery cost of 1%.
                break;
            case 0:
                cout << "no movement" << endl;
                // Assumption: no battery cost
                break;
            case 1:
                myRobot.forward();
                // Assumption: constant battery cost of 1%.
                break;
            default:
                cout << "unkown input" << endl;
                break;
        } 
    }
    cout << "Battery is already dead!" << endl;
    
    return 0; 
}


// Robot Class Implementation:
// Constructor: 
Robot::Robot(double currentBattery, int currentPosition, double currentCost)
{
    setBattery(currentBattery);
    setPosition(currentPosition);
    setCost(currentCost);
    cout << "Robot was set: ";
    print(); 
}
// Setters:
void Robot::setBattery(double currentBattery)
{
    battery = currentBattery; 
}
void Robot::setPosition(int currentPosition)
{
    position = currentPosition;
    setLocation(currentPosition);
}
void Robot::setCost(double currentCost)
{
    batteryCost = currentCost;
}
void Robot::setLocation(int currentPosition)
{
    location[0] = path[currentPosition][0];
    location[1] = path[currentPosition][1];
}
void Robot::print()
{
    cout << "Battery = " << battery 
         << " | Position = " << position 
         << " | Location = {" << location[0] << "," << location[1] << "}" << endl;
}
void Robot::forward()
{
    if (position >= pathLength-1)
        cout << "Can't move forward (boundary!)" << endl;
    else
    {
        setLocation(++position);
        battery -= batteryCost;
        cout << "Robot moved forward: ";
        print();
    }
}
void Robot::backward()
{
    if (position <= 0)
        cout << "Can't move backward (boundary!)" << endl;
    else
    {
        setLocation(--position);
        battery -= batteryCost;
        cout << "Robot moved backward: ";
        print();
    }
}
// Getters: 
double Robot::getBattery()
{
    return battery;
}
int Robot::getPosition()
{
    return position;
}
double Robot::getLocation()
{
    return location[2];
}
double Robot::getDistance(int currentPosition)
{
    // For further development !!
    return 1.1;
}
double Robot::getMoveCost(double distance)
{
    // For further development !!
    return 1.1;
}

