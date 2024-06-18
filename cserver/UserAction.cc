#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "GameController.h"
#include "UserAction.h"
#include <queue>

using namespace wfrest;
using namespace ctl;
using namespace std;

GameController& controller = GameController::getInstance("default");

// -- DON'T MODIFY ANY CODE ABOVE THIS LINE -- //
// -- YOU ARE ALLOW TO ADD HEADER FILES UNDER THIS LINE -- //

// TODO: Choose your recipe mode
// you may want to have different strategy for different recipe mode

const string RecipeMode = "salad";
//const string RecipeMode = "salad_cheeseburger";
//const string RecipeMode = "all";


// A template GLOBAL VARIABLE vector to store operations
// Feel free to modify this data structure! (or create your own to use)
queue<char> operations;

// A template map to store the position of each counter
// Question: How do you extend this map to store more than one position for a counter?
// Question2: cutting / normal counter have more than one position, how will you handle it?
std::map<Counter, std::vector<std::pair<int, int>>> counterPosition = {
    {BreadCounter, {{0, 1}}},
    {CabbageCounter, {{8, 4}}},
    {CheeseBlockCounter, {{0, 10}}},
    {TomatoCounter, {{8, 7}}},
    {RawPattyCounter, {{0, 20}}},
    {StoveCounter, {{0, 19}}},
    {PlatesCounter, {{3, 20}}},
    {TrashCounter, {{6, 20}}},
    {DeliveryCounter, {{1, 20}}},
    {CuttingCounter, {{{8, 14}, {0, 14}}}},
    {NormalCounter, {{{8, 12}, {0, 3}, {0, 6}, {0, 12}, {8, 20}, {8, 18}, {8, 10}, {8, 0}, {1, 0}, {6, 0}, {3, 0}}}}
};

const map<Counter, vector<char> > counterDirection = {
    { BreadCounter, {'w'} },
    { CabbageCounter, {'s'} },
    { CheeseBlockCounter, {'w'} },
    { TomatoCounter, {'s'} },
    { RawPattyCounter, {'w'} },
    { StoveCounter, {'w'} },
    { PlatesCounter, {'d'} },
    { TrashCounter, {'d'} },
    { DeliveryCounter, {'d'} },

    // There are 2 cutting counters, this is only one of it
    { CuttingCounter, {{'s'}, {'w'}} },
    // There are so many normal counters, this is only one of it
    { NormalCounter, {{'s'}, {'w'}, {'a'}, {'d'}} }
};

void DefaultInitialize();
void DefaultSendOperation();

// Init the game (DO NOT MODIFY THIS FUNCTION)
void UserAction::InitGame() {
    Initialize();
    // Set the response to "ok" when finished initialization
    controller.SetResponse("ok");
}

// Just a cute Initializing function
void UserAction::Initialize() {
    // TODO: You can do some initialization in this function.
    // Feel free to modify this function.
    // DefaultInitialize() will make you a MEGABurger!
    cout << "Initializing the game..." << endl;
    DefaultInitialize();
}

// Main Function of you game logic
void UserAction::SendOperation() {
    // TODO: Implement your gaming logic here
    // DefaultSendOperation() will make you a MEGABurger!
    void MakeSalad();
    //DefaultSendOperation();
}


// -- Moving series functions Below -- //

  // x then y
    
void MovePointToPoint(pair<int, int> from, pair<int, int> to) {
    // TODO: Move from one point to another point
    char xdir = from.first < to.first ? 's':'w';
    char ydir = from.second < to.second ? 'd':'a';
    
    int xcnt = abs(from.first - to.first);
    int ycnt = abs(from.second - to.second);

    for(int i = 0; i < xcnt; i++){
        operations.push(xdir);
    }

    for(int i = 0; i < ycnt; i++){
        operations.push(ydir);
    }
}

void MovePointToCounter(pair<int, int> fromPoint, Counter toCounter) {
    // TODO: Move from one point to a counter
    auto targetPosition = counterPosition.at(toCounter);
    MovePointToPoint(fromPoint, targetPosition);
}

void MoveCounterToCounter(Counter from, Counter to) {
    // TODO: Move from one counter to another counter
    auto fromPosition = counterPosition.at(from);
    if (!fromPosition.empty()) {
        MovePointToPoint(fromPosition[0], counterPosition.at(to)[0]);
    }
}

void MoveCounterToCounterAndinteract(Counter from, Counter to) {
    // almost done QQ
    auto fromPos = counterPosition.at(from);
    MoveCounterToCounter(fromPos, to);
    if (counterDirection.at(to)[0] == 'w') {
        operations.push('w');
    } else if (counterDirection.at(to)[0] == 's') {
        operations.push('s');
    } else if (counterDirection.at(to)[0] == 'a') {
        operations.push('a');
    } else if (counterDirection.at(to)[0] == 'd') {
        operations.push('d');
    }
    operations.push('e');
}

// -- Moving series functions Above -- //

// -- Strategy for different Recipe Mode Below -- //

Recipe SaladModeStrategy() {
    // Only salad will be in order in Salad Mode
    return Salad;
}

Recipe SaladCheeseburgerModeStrategy() {
    // TODO: Implement your salad cheeseburger mode strategy here
    // Below is a simple example, feel free to change it

    vector<Order> orderList = controller.GetOrderList();
    if (orderList.empty()) {
        return Salad;
    }
    return orderList[0].recipe;
}

Recipe AllRecipeModeStrategy() {
    // TODO: Implement your all recipe mode strategy here
    // Below is a simple example, feel free to change it

    return MegaBurger;
}

Recipe GetNextOrder() {
    // Return the next order based on the Recipe Mode
    if (RecipeMode == "salad") {
        return SaladModeStrategy();
    } else if (RecipeMode == "salad_cheeseburger") {
        return SaladCheeseburgerModeStrategy();
    } else {
        return AllRecipeModeStrategy();
    }
}

// -- Strategy for different Recipe Mode Above -- //

// -- Miscallaneous functions Below -- //

void CutIngredient(int times) {
    // TODO: Cut the Ingredient for times --> DONE
    for(int i = 0; i < times; i++){
        operations.push('f');
    }
    
}

// -- Miscallaneous functions Above -- //

// -- Pipeline Funtions Below -- //

// You are welcome to change the function prototype
// Like changing the return type or adding more parameters
void MakeSalad() {//--> Done
    // TODO: Salad Making Pipeline
    // Move To Cabbage Counter
    MovePointToCounter(controller.GetPlayerPosition(),CabbageCounter);
    // Grab Cabbage
    operations.push('e');
    // Move To Cutting Counter
    MoveCounterToCounterAndinteract(CabbageCounter,CuttingCounter);
    // Cut Cabbage
    CutIngredient(5);
    // Grab Cabbage Slices
    operations.push('e');
    // Move To A Normal Counter
    MoveCounterToCounterAndinteract(CuttingCounter,NormalCounter);
    // Put Down Cabbage Slices
    // Move To Tomato Counter
    MoveCounterToCounterAndinteract(NormalCounter,TomatoCounter);
    // Grab Tomato
    // Move To A Cutting Counter
    MoveCounterToCounterAndinteract(TomatoCounter,CuttingCounter);
    // Cut Tomato
    CutIngredient(3);
    // Move To Plate Counter
    MoveCounterToCounterAndinteract(CuttingCounter,PlatesCounter);
    // Grab Plate
    // Move To A Cutting Counter
    MoveCounterToCounterAndinteract(PlatesCounter,CuttingCounter);
    // Grab Tomato Slices
    // Move To A Normal Counter
    MoveCounterToCounterAndinteract(CuttingCounter,NormalCounter);
    // Grab Cabbage Slices
    // Move To Delivery Counter
    MoveCounterToCounterAndinteract(NormalCounter,DeliveryCounter);
    // Send Order!
}



void MakeBurger() {
    //TODO: 
    MovePointToCounter(controller.GetPlayerPosition(),RawPattyCounter);
    operations.push('e');//拿肉
    MoveCounterToCounterAndinteract(RawPattyCounter,StoveCounter);//煎
    MoveCounterToCounterAndinteract(StoveCounter,BreadCounter);//拿麵包
    MoveCounterToCounterAndinteract(BreadCounter,NormalCounter);//放到桌子上
    MoveCounterToCounterAndinteract(NormalCounter,PlatesCounter);//拿盤子
    MoveCounterToCounterAndinteract(PlatesCounter,StoveCounter);//裝肉
    MoveCounterToCounterAndinteract(StoveCounter,NormalCounter);//裝麵包
    MoveCounterToCounterAndinteract(NormalCounter,DeliveryCounter);//送
}
void MakeCheeseBurger() {
    //TODO: 
    MovePointToCounter(controller.GetPlayerPosition(),RawPattyCounter);
    operations.push('e');//拿肉
    MoveCounterToCounterAndinteract(RawPattyCounter,StoveCounter);//煎
    MoveCounterToCounterAndinteract(StoveCounter,CheeseBlockCounter);//拿起司
    MoveCounterToCounterAndinteract(CheeseBlockCounter,CuttingCounter);
    CutIngredient(3);//切起司
    MoveCounterToCounterAndinteract(CuttingCounter,BreadCounter);//拿麵包
    MoveCounterToCounterAndinteract(BreadCounter,NormalCounter);//放到桌子上
    MoveCounterToCounterAndinteract(NormalCounter,PlatesCounter);//拿盤子
    MoveCounterToCounterAndinteract(PlatesCounter,StoveCounter);//裝肉
    MoveCounterToCounterAndinteract(StoveCounter,CuttingCounter);//裝起司
    MoveCounterToCounterAndinteract(CuttingCounter,NormalCounter);//裝麵包
    MoveCounterToCounterAndinteract(NormalCounter,DeliveryCounter);//送
}
void MakeMegaBurger() {
    //TODO:
    MovePointToCounter(controller.GetPlayerPosition(),RawPattyCounter);
    operations.push('e');//拿肉
    MoveCounterToCounterAndinteract(RawPattyCounter,StoveCounter);//煎
    MoveCounterToCounterAndinteract(StoveCounter,CheeseBlockCounter);//拿起司
    MoveCounterToCounterAndinteract(CheeseBlockCounter,CuttingCounter[1]);
    CutIngredient(3);//切起司
    MoveCounterToCounterAndinteract(CuttingCounter[1],BreadCounter);//拿麵包
    MoveCounterToCounterAndinteract(BreadCounter,NormalCounter[3]);//放到(0,12)的桌子
    MoveCounterToCounterAndinteract(NormalCounter[3],PlatesCounter);//拿盤子
    MoveCounterToCounterAndinteract(PlatesCounter,StoveCounter);//裝肉
    MoveCounterToCounterAndinteract(StoveCounter,CuttingCounter[1]);//裝起司
    MoveCounterToCounterAndinteract(CuttingCounter[1],NormalCounter[3]);//裝麵包
    MoveCounterToCounterAndinteract(NormalCounter[3],NormalCounter[6]);//放到(8,10)的桌子
    MoveCounterToCounterAndinteract(NormalCounter[6],TomatoCounter);//拿番茄
    MoveCounterToCounterAndinteract(TomatoCounter,CuttingCounter[0]);//切番茄
    CutIngredient(3);
    MoveCounterToCounterAndinteract(CuttingCounter[0],NormalCounter[0]);//放到(8,12)的桌子
    MoveCounterToCounterAndinteract(NormalCounter[0],CabbageCounter);//拿菜
    MoveCounterToCounterAndinteract(CabbageCounter,CuttingCounter[0]);//切菜
    CutIngredient(5);
    MoveCounterToCounterAndinteract(CuttingCounter[0],NormalCounter[6]);//回到(8,10)有裝盤子的桌子
    MoveCounterToCounterAndinteract(NormalCounter[6],NormalCounter[0]);//裝番茄
    MoveCounterToCounterAndinteract(NormalCounter[0],CuttingCounter[0]);//裝菜
    MoveCounterToCounterAndinteract(CuttingCounter[0],DeliveryCounter);//送
}

// -- Pipeline Funtions Below -- //

// -- Default Series Function Below -- //

// SendOperation function template, free MEGABurger for you!
void DefaultSendOperation() {
    char s;
    if (!operations.empty()) {
        operations.push(s);
        operations.pop();
        cout << "Operation: " << s << endl;
    }
    if (s == 'w') controller.MoveUp();
    if (s == 's') controller.MoveDown();
    if (s == 'a') controller.MoveLeft();
    if (s == 'd') controller.MoveRight();
    if (s == 'e') controller.Interact();
    if (s == 'f') controller.InteractSpecial();
}

void DefaultInitialize() {

    vector<char> initOps = {
        'w', 'w', 'w', 'e', 'e', 'd', 'd', 'd', 'w', 'e', 'f', 'e', 'f', // grab cheese and cut it
        'd', 'd', 'd', 'd', 'd', 'd', 'w', 'e', 'a', 'w', 'e', 's', 's', 'd', 'e', // fry meat and grab plate
        'a', 'a', 'a', 'a', 'w', 'w', 'e', 'd', 'w', 'e', // grab cheese slices and cooked meat
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', // wait for the meat to be cooked
        'a', 'a', 'a', 'a', 'w', 'e', // put the plate down
        's', 's', 's', 's', 's', 's', 's', 's', // move down
        'a', 'a', 'a', 'a', 'a', 'a', 's', 'e', // grab tomato
        'd', 'd', 'd', 'd', 'd', 'd', 'd', 's', 'e', 'f', 'f', 'f', // cut tomato
        'e', 'a', 's', 'e', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 's', 'e', // grab lettuce
        'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', // move to cutting counter
        's', 'e', 'f', 'f', 'f', 'f', 'f', // cut lettuce
        'e', 'a', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'e', 'e', // grab plate
        's', 's', 's', 's', 's', 's', 's', 's', // move down
        'e', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', // move to bottom left
        'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', // move to bread counter
        'd', 'w', 'e', 'a', 'w', 'e', 'd', 'w', 'e', // grab bread
        'e', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd',
        'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'e' //finish
    };
    for (char op : initOps) {
        operations.push(op);
    }

}
