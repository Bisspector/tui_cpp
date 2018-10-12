#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <string>
#include "execution.h"
#include "crow.h"
/////
#include <bits/stdc++.h>

using namespace std;
using namespace crow;

struct car{
    std :: string ID;
    std :: string qual;
};

struct Orders{
    std :: string name;
    std :: string phone;
    std :: vector <car> Cart;
    bool used;
};


std :: map <string, bool> used;

std :: map <std :: string, int> OrdUse;

void start() {
    int rc = sqlite3_open("./restaurant.db", &db);
    if (rc) {
        cout << "Can't open database: \n" << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "Opened database successfully\n";
    }
    createTable("menu", {"id", "name", "price", "time", "description", "image"}, {"intkeynn", "stringnn", "stringnn", "stringnn", "stringnn", "stringnn"});
    createTable("orders", {"id", "description"}, {"stringnn", "stringnn"});
}

// void start_order() {
//     int rc = sqlite3_open("./order.db", &db);
//     if (rc) {
//         cout << "Can't open order's data base!!!\n" << sqlite3_errmsg(db) << '\n';
//     } else {
//         cout << "Order's data base opened successfully!!!\n";
//     }
//     createTable("orders", {"id", "description"}, {"stringnn", "stringnn"});
// }

std :: vector <std :: string> DishVec;
std :: vector <std :: string> DishArray;

std :: vector <Orders> OrderVec;
std :: vector <std :: string> OrderArray;

std :: string MinusSpace(std :: string s){
    while (s[0] == ' '){
        s.erase(0, 1);
    }
    return s;
}

int Min(int a, int b){
    if (a <= b)
        return a;
    return b;
}

void Convert_to_json(std :: string dish0, std :: string dish, std :: string price0, std :: string price, std :: string image0, std :: string image, std :: string description0, std :: string description, std :: string time0, std :: string time, std :: string ID0, std :: string ID){
    std :: string rez = "{ \"" + dish0 + "\":\"" + dish + "\", \"" + price0 + "\":\"" + price + "\", \"" + image0 + "\":\"" + image  + "\", \"" + description0 + "\":\"" + description + "\", \"" + time0 + "\":\"" + time + "\", \"" + ID0 + "\":\"" + ID + "\" }" ;
    std :: cout << "\n" << rez;
    if (!used[rez])
        DishVec.push_back(rez);
    used[rez] = true;
}

std :: string Convert_to_json_write(std :: string dish0, std :: string dish, std :: string price0, std :: string price, std :: string image0, std :: string image, std :: string description0, std :: string description, std :: string time0, std :: string time, std :: string ID0, std :: string ID){
    std :: string rez = "{ \"" + dish0 + "\":\"" + dish + "\", \"" + price0 + "\":\"" + price + "\", \"" + image0 + "\":\"" + image  + "\", \"" + description0 + "\":\"" + description + "\", \"" + time0 + "\":\"" + time + "\", \"" + ID0 + "\":\"" + ID + "\" }" ;
    std :: cout << "\n" << rez;
    return rez;
}

//%%%%%%%%%%%%%%%%%%%%

void Order_Processing_json(std :: string name0, std :: string name, std :: string phone0, std :: string phone, std::vector<car> car0){
    cout << "\nOrder_Processing_json:successfully\n";
    std :: string rez = "";
    rez += "{ \"" + name0 + "\":\"" + name + "\", \"" + phone0 + "\":\"" + phone + "\", \"order_description\":[ ";
    for (int i = 0; i < car0.size(); i++){
        if (i != car0.size() - 1){
            rez += "{ \"ID\":\"" + car0[i].ID + "\", \"quantity\":\"" + car0[i].qual + "\" }, ";
        } else {
            rez += "{ \"ID\":\"" + car0[i].ID + "\", \"quantity\":\"" + car0[i].qual + "\" }";
        }
    }
    rez += " ] }";
    cout << "\nrez:" << rez << "\n";
    OrderArray.push_back(rez);
}

void Order_Processing_comand(std :: string s){
    cout << "\nOrder_Processing_comand: successfully\n";
    cout << "\nOPc:" << s << "\n";
    s+= "_";
    Orders ord;
    ord.name = "";
    ord.phone = "";
    ord.Cart = { };
    std :: string car1 = "";
    std :: string car2 = "";
    int top = 0;
    int front = 0;
    std :: string resault = "";
    for (int i = 0; i < s.length(); i++){
        if (s[i] != '_'){
            if (s[i] == '-'){
                s[i] = ' ';
            }
        } else {
            top++;
            if (top == 1){
                ord.name = s.substr(front, i - front);
            }
            if (top == 2){
                ord.phone = s.substr(front, i - front);
            }
            if (top >= 3 && top % 2 == 1){
                car1 = "";
                // ord.Cart.push_back(s.substr(front, i - front));
                car1 += s.substr(front, i - front);
                resault += car1 + "_";
            }
            if (top >= 3 && top % 2 == 0){
                car2 = "";
                car2 += s.substr(front, i - front);
                resault += car2 + "_";
                ord.Cart.push_back({car1, car2});
            }
            front = i + 1;
        }
        
    }
    cout << "\n" << ord.name + ord.phone + car1 + car2 << "\n";
//

//
    OrdUse[ord.name + ord.phone + car1 + car2]++;
    if (OrdUse[ord.name + ord.phone + car1 + car2] <= 1){
        // OrderVec[ord.name + ord.phone + car1 + car2].used = true;
        add("orders", {ord.name + ord.phone, resault});
        Order_Processing_json("name", ord.name, "phone", ord.phone, ord.Cart);
        OrderVec.push_back({ord.name, ord.phone, ord.Cart});
    }
    // if (!OrderVec[ord.name + ord.phone + car1 + car2].used){
    //     OrderVec.push_back({ord.name, ord.phone, ord.Cart});
    // }
}


std :: string ReturnOrderInfoJson(std :: string name0, std :: string name, std :: string phone0, std :: string phone, std::vector<car> car0){
    cout << "\nOrder_Processing_json:successfully\n";
    std :: string rez = "";
    rez += "{ \"" + name0 + "\":\"" + name + "\", \"" + phone0 + "\":\"" + phone + "\", \"order_description\":[ ";
    for (int i = 0; i < car0.size(); i++){
        if (i != car0.size() - 1){
            rez += "{ \"ID\":\"" + car0[i].ID + "\", \"quantity\":\"" + car0[i].qual + "\" }, ";
        } else {
            rez += "{ \"ID\":\"" + car0[i].ID + "\", \"quantity\":\"" + car0[i].qual + "\" }";
        }
    }
    rez += " ] }";
    cout << "\nrez:" << rez << "\n";
    return rez;
}


std :: vector<car> ReturnOrderInfoComand(std :: string s){
    s += "_";
    int top = 0;
    int front = 0;
    std :: vector <car> res;
    for (int i = 0; i < s.length(); i++){
        if (s[i] != '_'){
            if (s[i] == '-'){
                s[i] = ' ';
            }
        } else {
            top++;
            if (top % 2 == 1){
                res.push_back({});
                res[res.size() - 1].ID = s.substr(front, i - front);
            }
            if (top % 2 == 0){
                res[res.size() - 1].qual = s.substr(front, i - front);
            }
            front = i + 1;
        }
    }
    return res;
}


//%%%%%%%%%%%%%%%%%%%%

void Convert_to_comand(std :: string s){
    s += '_';
    std :: string name = "";
    std :: string price = "";
    std :: string time = "";
    std :: string description = "";
    std :: string image = "./images/";
    std :: string ID = "";
    int top = 0, front = 0;
    for (int i = 0; i < s.size(); i++){
        if (s[i] != '_'){
            if (s[i] == '-')
                s[i] = ' ';
        } else {
            top++;
            cout << "\n\n\n" << top << "\n\n\n";
            if (top == 1){
                name = s.substr(front, i - front);
            }
            if (top == 2){
                price = s.substr(front, i - front);
            }
            if (top == 3){
                time = s.substr(front, i - front);
            }
            if (top == 4){
                description = s.substr(front, i - front);
            }
            if (top == 5){
                image += s.substr(front, i - front) + ".png";
            }
            if (top == 6){
                ID += s.substr(front, i - front);
            }
            front = i + 1;
        }
    }
    Convert_to_json("name", name, "price", price, "image", image, "description", description, "time", time, "ID", ID);
    // if (used[DishVec[DishVec.size() - 1]]){
        add("menu", {ID, name, price, time, description, image});    
    // }
}

/*    Тут будут написаны некоторые примеры                           */
/*    add("menu", {"1", "pizza", "23", "1"});                        */
/*    update("menu", "id", "1", {"1", "soupe", "13", str(1)})        */
/*    delete("menu", "id", "1")                                      */
/*    take("menu", "id", "3", "price") - взять по id цену            */
/*-------------------------------------------------------------------*/

int main() {

    std :: string AddMeal = "";
    bool need;
    start();
    


    crow::App<> app;

    ///


    

    CROW_ROUTE(app, "/add/<string>")
        .methods("GET"_method, "POST"_method, "DELETE"_method)
        ([&AddMeal, &need](const request& req, const string& id) {
            if (req.method == "GET"_method)
            {
                if ((req.url_params.get("v") != nullptr) & (req.url_params.get("q") != nullptr))
                {
                    // ...
                }
                if (AddMeal != id){
                    need = true;
                }
                if (AddMeal == id){
                    need = false;
                }
                AddMeal = id;
                if (need == true){
                    cout << "\n\n\n\n\n" << AddMeal << "\n\n\n\n\n";
                    Convert_to_comand(AddMeal);
                }

                return response(200, "You used GET");
            }
            else if (req.method == "POST"_method)
            {
                return response(200, "You used POST");
            }
            else if (req.method == "DELETE"_method)
            {
                return response(200, "You used DELETE");
            }
            else
            {
                return response(404);
            }
        });
    string write_info = "";
    ///
    CROW_ROUTE(app, "/info/<string>")
        .methods("GET"_method, "POST"_method, "DELETE"_method)
        ([&write_info](const request& req, const string& id) {
            // write_info = id;
            if (req.method == "GET"_method)
            {
                write_info = id;
                return response(200, "You used GET");
            }
            else if (req.method == "POST"_method)
            {
                return response(200, "You used POST");
            }
            else if (req.method == "DELETE"_method)
            {
                return response(200, "You used DELETE");
            }
            else
            {
                return response(404);
            }
        });
    ///

    ///

    CROW_ROUTE(app, "/write_info")([&write_info](const crow::request&, crow::response& res) -> void{
        res.add_header("Access-Control-Allow-Origin", "*");
        ///
        res.write("[ ");
        //
        string price = take("menu", "id", write_info, "price");
        cout << "\n" << price << "\n\n";
        string name = take("menu", "id", write_info, "name");
        cout << "\n" << name << "\n\n";
        string image = take("menu", "id", write_info, "image");
        cout << "\n" << image << "\n\n";
        string description = take("menu", "id", write_info, "description");
        cout << "\n" << description << "\n\n";
        string time = take("menu", "id", write_info, "time");
        cout << "\n" << time << "\n\n";
        res.write(Convert_to_json_write("name", name, "price", price, "image", image, "description", description, "time", time, "ID", write_info));
        //
        res.write(" ]");
        ///
        res.end();
    });

    ///

    CROW_ROUTE(app, "/menu")([](const crow::request&, crow::response& res) -> void{
        res.add_header("Access-Control-Allow-Origin", "*");

        // start();


        // add("menu", {"1", "pizza", "23", "1", "tasty thing", "../image/png1.png"});
        // int ColAsk = 2;
        //dropTable("menu");
        //add("menu", {"1", "pizza", "23", "1", "tasty thing", "../image/png1.png"});
        //int ColAsk = 2;
        string name = "";
        string price = "";
        string time = "";
        string description = "";
        string image = "./image/";
        // name = take("menu", "id", "1", "price");
        // price = take("menu", "id", "1", "name");
        // time = take("menu", "id", "1", "time");
        // description = take("menu", "id", "1", "description");
        // image = take("menu", "id", "1", "image");
        cout << name << ' ' << price << ' ' << time << ' ' << description << ' ' << image << '\n';

        res.write("[ ");
        for (int i = 0; i < DishVec.size(); i++){
            // Convert_to_json("name", name, "price", price, "image", image, "description", description, "time", time);
            res.write(DishVec[i]);
            if (i != DishVec.size() - 1){
                res.write(", ");
            }
        }

        
        res.write(" ]");
        res.end();
    });

    std :: string order_info = "";

    CROW_ROUTE(app, "/add_order/<string>")
        .methods("GET"_method, "POST"_method, "DELETE"_method)
        ([&order_info](const request& req, const string& id) {
            
            if (req.method == "GET"_method)
            {
                // order_info = id;
                cout << "\n\nid:" << id << "\n\n";
                Order_Processing_comand(id);
                return response(200, "You used GET");
            }
            else if (req.method == "POST"_method)
            {
                return response(200, "You used POST");
            }
            else if (req.method == "DELETE"_method)
            {
                return response(200, "You used DELETE");
            }
            else
            {
                return response(404);
            }
        });
    // Order_Processing_comand(order_info);


    CROW_ROUTE(app, "/order_menu")([](const crow::request&, crow::response& res) -> void{
        res.add_header("Access-Control-Allow-Origin", "*");
        // start_order();

        res.write("[ ");
        cout << "\n\n\nOrderArray: " << OrderArray.size() << "\n\n\n";
        for (int i = 0; i < OrderArray.size(); i++){
            if (i != OrderArray.size() - 1){
                res.write(OrderArray[i] + ", ");
            } else {
                res.write(OrderArray[i]);
            }
        }

        res.write(" ]");
        res.end();
    });

    CROW_ROUTE(app, "/info_order/<string>")
        .methods("GET"_method, "POST"_method, "DELETE"_method)
        ([&order_info](const request& req, const string& id) {
            
            if (req.method == "GET"_method)
            {
                order_info = id;
                return response(200, "You used GET");
            }
            else if (req.method == "POST"_method)
            {
                return response(200, "You used POST");
            }
            else if (req.method == "DELETE"_method)
            {
                return response(200, "You used DELETE");
            }
            else
            {
                return response(404);
            }
        });



    CROW_ROUTE(app, "/write_order_info")([&order_info](const crow::request&, crow::response& res) -> void{
        res.add_header("Access-Control-Allow-Origin", "*");
        // start_order();

        res.write("[ ");
        
        std :: string description = order_info;
        std :: string name = "";
        std :: string phone = "";
        for (int i = 0; i < order_info.length(); i++){
            if (order_info[i] == '_'){
                name = order_info.substr(0, i);
                phone = order_info.substr(i + 1, order_info.length() - i - 1);
            }
        }
        cout << "\n\n\n\ndescription:" << description << "\n\n\n\n";
        res.write(ReturnOrderInfoJson("name", name, "phone", phone, ReturnOrderInfoComand(description)));


        res.write(" ]");
        res.end();
    });

    

    app.port(18080)
        .multithreaded()
        .run();

    //dropTable("menu");
    return 0;
}
