//
// Created by osse on 2019-05-09.
//
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <cstdio>
#include <string>
#include <iostream>
#include "vector"
#include <unistd.h>
#include <chrono>
#include <thread>
//#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <nlohmann/json.hpp>
#include "algorithm"

//#include <stdlib.h>


static std::string buffer;
#ifndef UNTITLED1_cUrlAndJsoncpp_H
#define UNTITLED1_cUrlAndJsoncpp_H


class cUrlAndJsoncpp
{
private:

    //strukt för att spara alla flygplatser kod
    struct airport
    {
        std::string airportCode, airportName, country;
    };
    std::vector<airport> allAirports;

    //den nyckeln kan användas för 100 anrop
    std::string aviationStupidKey="bf4342-2f7cb7";
//    std::string aviationStupidKey="7a2c90-e96924";


    int saveTempToSendStockholm{0};
    int saveTempToSendLondon{0};

    std::string saveCityToSendStockholm;
    std::string saveCityToSendLondon;
    float saveUSDToSend,saveEURToSend;

    void getAllAirports(); //hämta alla flygplataser kod
    void getArlandaDeparture(); //hämta arlanda API "departure"

    void getWeatherData(std::string city); //hämta väder
    void getExchangeRatekData(); //hämta valuta
    void static getTrafikData(); //hämta SL information

    void static readMockApi();
    void sendMockApiTemp();
    void sendMockApiCurrency();

    static void headerAndOtherStuff(std::string uri, std::string request); //några rader som upprepas


    static size_t curl_write(void *ptr, size_t size, size_t nmemb); //en funktion som läser och sparar API data
    int kbhit(); // key hit för att stoppa programmet
    void nap(int timeToSleep); //sleep funktion med key hit


public:
    void init();
};

#endif //UNTITLED1_cUrlAndJsoncpp_H
