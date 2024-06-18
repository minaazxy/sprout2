#include <map>
using namespace std;

enum Items {
    // Non-edible
    Plate,

    // Raw Ingredients
    Cabbage,
    CheeseBlock,
    RawPatty,
    Tomato,

    // Processed Ingredients
    Bread,
    CabbageSlices,
    CheeseSlices,
    CookedPatty,
    TomatoSlices,

    // Overcooked Ingredients
    BurntPatty,
};

enum Recipe {
    Salad,
    Burger,
    CheeseBurger,
    MegaBurger,
};

enum FryingState {
    Idle,
    Frying,
    Fried,
    Burnt,
};

enum Counter {
    // Ingredient counters
    BreadCounter,
    CabbageCounter,
    CheeseBlockCounter,
    RawPattyCounter,
    TomatoCounter,

    // Tool counters
    CuttingCounter,
    StoveCounter,
    NormalCounter,
    PlatesCounter,
    TrashCounter,
    DeliveryCounter,
};

struct Order {
    // Properties
    int orderID;
    Recipe recipe;
    int score;
    int arrivalRound;

    // Constructors
    Order() { orderID = -1; }
    Order(int _orderID, Recipe _recipe, int _score, int _arrivalRound) {
        orderID = _orderID,
        recipe = _recipe,
        score = _score,
        arrivalRound = _arrivalRound;
     }
};

const map<Items, string> ItemsMap = {
    { Plate, "Plate" },
    { Cabbage, "Cabbage" },
    { CheeseBlock, "CheeseBlock" },
    { RawPatty, "RawPatty" },
    { Tomato, "Tomato" },
    { Bread, "Bread" },
    { CabbageSlices, "CabbageSlices" },
    { CheeseSlices, "CheeseSlices" },
    { CookedPatty, "CookedPatty" },
    { TomatoSlices, "TomatoSlices" },
    { BurntPatty, "BurntPatty" },
};

const map<Recipe, string> RecipeMap = {
    { Salad, "Salad" },
    { Burger, "Burger" },
    { CheeseBurger, "CheeseBurger" },
    { MegaBurger, "MegaBurger" },
};

const map<FryingState, string> FryingStateMap = {
    { Idle, "Idle" },
    { Frying, "Frying" },
    { Fried, "Fried" },
    { Burnt, "Burnt" },
};

const map<Recipe, vector<Items>> RecipeIngredients = {
    { Salad, { CabbageSlices, TomatoSlices } },
    { Burger, { Bread, CookedPatty } },
    { CheeseBurger, { Bread, CheeseSlices, CookedPatty } },
    { MegaBurger, { Bread, CabbageSlices, CheeseSlices, CookedPatty, TomatoSlices } },
};
