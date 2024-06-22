#include <csignal>
#include <iostream>
#include <random>
#include <vector>
#include <map>

#include "GameController.h"
#include "UserAction.h"
#include "wfrest/HttpServer.h"
#include "workflow/WFFacilities.h"

using namespace wfrest;
using namespace ctl;
using namespace std;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo) { wait_group.done(); }

mt19937 ran(time(nullptr));

string RandomString();

int main() {
    signal(SIGINT, sig_handler);

    HttpServer svr;

    string responseString = "default";
    GameController& controller = GameController::getInstance(responseString);

    UserAction action;

    // init game
    svr.GET("/api/init", [&](const HttpReq *req, HttpResp *resp) {
        action.InitGame();

        resp->String(controller.GetResponse());
        // CORS
        resp->add_header_pair("Access-Control-Allow-Origin", "*");
    });

    // gaming operations (move right, move left, move up, move down, interact, interact special)
    svr.GET("/api/operation", [&](const HttpReq *req, HttpResp *resp) {
        // cout << "Round: " << req->query("round") << endl;
        action.SendOperation();

        resp->String(controller.GetResponse());
        // CORS
        resp->add_header_pair("Access-Control-Allow-Origin", "*");
    });

    // gaming events (e.g. recipe delivered, recipe failed, recipe list, total score, time left, overcooked warning)

    // javascript will send a OPTIONS request before sending a POST request
    // deal with the OPTIONS request here

    svr.ROUTE("/api/events", [&](const HttpReq *req, HttpResp *resp) {
        if (string(req->get_method()) == "OPTIONS") {
            // CORS
            resp->add_header_pair("Access-Control-Allow-Origin", "*");
            resp->add_header_pair("Access-Control-Allow-Methods", "POST");
            resp->add_header_pair("Access-Control-Allow-Headers", "Content-Type");
            return;
        }
    }, {"OPTIONS"});

    svr.POST("/api/events", [&](const HttpReq *req, HttpResp *resp) {
        // cout << "Received events: " << req->json() << endl;

        controller.ReceiveEvents(req->json());

        resp->String(controller.GetResponse());
        // CORS
        resp->add_header_pair("Access-Control-Allow-Origin", "*");
    });

    // curl -v http://ip:port/hello
    svr.GET("/hello", [](const HttpReq *req, HttpResp *resp) {
        resp->String("world\n");
    });

    // curl -v http://ip:port/data
    svr.GET("/data", [](const HttpReq *req, HttpResp *resp) {
        string str = "Hello world";
        resp->String(move(str));
        // CORS
        resp->add_header_pair("Access-Control-Allow-Origin", "*");
    });

    svr.ROUTE("/multi",
              [](const HttpReq *req, HttpResp *resp) {
                  string method(req->get_method());
                  resp->String(move(method));
              },
              {"GET", "POST"});

    // curl -v http://ip:port/post -d 'post hello world'
    svr.POST("/post", [](const HttpReq *req, HttpResp *resp) {
        // reference, no copy here
        string &body = req->body();
        fprintf(stderr, "post data : %s\n", body.c_str());
    });

    // curl -v http://ip:port/any_path
    svr.set_default_route("/data");

    if (svr.track().start(8887) == 0) {
        svr.list_routes();
        wait_group.wait();
        svr.stop();
    } else {
        fprintf(stderr, "Cannot start server");
        exit(1);
    }
    return 0;
}

string RandomString() {
    string charset = "abcdefghijklmnopqrstuvwxyz";
    string str;
    for (int i = 0; i < 10; i++) {
        str += charset[ran() % 26];
    }
    return str;
}
