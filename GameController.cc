#include "GameController.h"

using namespace std;
using namespace ctl;

void GameController::ReceiveEvents(const wfrest::Json &json) {
    // DO NOT MODIFY THIS FUNCTION
    // Parse the JSON and update the game state

    GameController::SetRound(int(json["Round"]));
    GameController::SetTotalScore(int(json["TotalScore"]));

    GameController::SetPlayerPosition(pair<int, int>(int(json["PlayerPosition"]["x"]), int(json["PlayerPosition"]["y"])));

    vector<Items> playerHoldItems;
    for (auto item : json["PlayerHoldItems"]) {
        Items itemEnum = (Items)int(item);
        playerHoldItems.emplace_back(itemEnum);
    }
    GameController::SetPlayerHoldItems(playerHoldItems);

    pair<int, int> orderDelivered = pair<int, int>(
        int(json["OrderDelivered"][0]), int(json["OrderDelivered"][1])
    );
    GameController::SetOrderDelivered(orderDelivered);

    // New Recipe
    Order order = Order(
        int(json["NewOrder"]["orderID"]), 
        (Recipe)int(json["NewOrder"]["recipeID"]),
        int(json["NewOrder"]["orderScore"]),
        int(json["NewOrder"]["existedTime"])
    );
    GameController::SetNewOrder(order);

    // Recipe List
    vector<Order> orderList;
    for (auto orderInfo : json["OrderList"]) {
        Order order = Order(
            int(orderInfo["orderID"]), 
            (Recipe)int(orderInfo["recipeID"]), 
            int(orderInfo["orderScore"]),
            int(orderInfo["existedTime"])
        );
        orderList.emplace_back(order);
    }
    GameController::SetOrderList(orderList);

    // Frying Timer and Frying State
    GameController::SetFryingTimer(int(json["FryingTimer"]));
    GameController::SetFryingState((FryingState)int(json["FryingState"]));

    PrintEvents();

    // Update the response string
    resp = "ok";
}

void GameController::PrintItems(vector<Items> items) {
    bool first = true;
    cout << "[ ";
    for (auto item : items) {
        if (first) first = false;
        else cout << ", ";
        cout << ItemsMap.at(item);
    }
    cout << " ]" << endl;
}

void GameController::PrintOrderInfo(Order order) {
    cout << "> ";
    cout << "Order ID: " << order.orderID << ", ";
    cout << "Recipe: " << RecipeMap.at(order.recipe) << ", ";
    cout << "Score: " << order.score << ", ";
    cout << "Arrial Time: " << order.arrivalRound << endl;
}

void GameController::PrintEvents() {
    cout << "--- Current Game State ---" << endl;

    cout << "Round: " << GameController::GetRound() << endl;
    cout << "Total Score: " << GameController::GetTotalScore() << endl;

    cout << "Player Position: (" << GameController::GetPlayerPosition().first << ", " << GameController::GetPlayerPosition().second << ")" << endl;
    cout << "Player Hold Items: ";
    GameController::PrintItems(GameController::GetPlayerHoldItems());

    cout << "New Order: ";
    GameController::PrintOrderInfo(GameController::GetNewOrder());

    cout << "Order List: " << endl;
    for (auto order : GameController::GetOrderList()) {
        GameController::PrintOrderInfo(order);
    }

    cout << "Frying Timer: " << GameController::GetFryingTimer() << endl;
    cout << "Frying State: " << FryingStateMap.at(GameController::GetFryingState()) << endl;
    

    cout << "--- End of Game State ---" << endl;
}