//
// Created by osse on 2019-05-09.
//
#include "cUrlAndJsoncpp.h"

//public functions
void cUrlAndJsoncpp::init()
{
    std::cout << "Initiaizing ...\n";
    getAllAirports(); //hämta alla flygplatser kod och sparar dom

    bool run{true};
    std::cout << "Ready to go   ...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // std::cin.ignore(1024, '\n');
    while (run)
    {
        while (!kbhit())
        {
            system("clear");
            std::cout << "Press p to enter the menu : \n";
            std::cout << "Currency : " << std::endl;
            getExchangeRatekData(); //hämta valuta
            std::cout << "Weather" << std::endl;
            getWeatherData("Stockholm");//hämta väder stockholm
            getWeatherData("Paris");//hämta väder paris
            getWeatherData("London");//hämta väder london
            getWeatherData("hawaii");//hämta väder hawaii
            std::cout << "\nBusses, Arlanda terminal 5" << std::endl;
            getTrafikData();//hämta SL data
            std::cout << "Flights, Arlanda" << std::endl;
            getArlandaDeparture();//hämta arlanda data


            nap(3000);


        }
        if (getchar()) //om användaren trycker "p"
        {


            //std::cin.ignore(1024, '\n');
            system("clear");

            // fseek(stdin, 0, SEEK_END);
            std::cout << "Choose what do you want to do :\n";
            std::cout << "1-Send temperature data(stockholm).\n";
            std::cout << "2-Read all temperature data(stockholm).\n";
            std::cout << "0-Exit\n";
            int userInput;
            std::cin >> userInput;
            //send temperature data

            if (userInput == 1)// 1 skicka data till mockAPI
            {
                sendMockApiTemp();
                sendMockApiCurrency();
                std::cin.ignore(1024, '\n');
                std::cout << "Press enter to continue...";
                // std::cin.get();
            }
                //read temperature data
            else if (userInput == 2)// 2 hämta data från mockAPI
            {
                readMockApi();
                std::cin.ignore(1024, '\n');
                std::cout << "Press enter to continue...";
                //std::cin.get();
            } else if (userInput == 0)//stänga programmet
            {
                run = false;
                std::cout << "closing \n";
            } else
            {
                std::cout << "wrong input.\a\n ";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            }
            std::cin.ignore(1024, '\n');

        }

    }

}

///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
//private functions
void cUrlAndJsoncpp::getTrafikData()
{

    //hämta API
    std::string uri("http://api.sl.se/api2/realtimedeparturesV4.json?key=b957ef223ec749ec9eecc88db0a3b7a9&siteid=5005");//uri

    headerAndOtherStuff(uri, "GET");


    //parsa API
    Json::Value jsonData;
    Json::Reader jsonReader;
    if (jsonReader.parse(buffer.c_str(), jsonData))
    {
        buffer.clear();
        //std::cout<<jsonData.toStyledString()<<std::endl<<std::endl;
        for (unsigned int i = 0; i < ((jsonData["ResponseData"]["Buses"].size() >= 6) ? 6
                                                                                      : (jsonData["ResponseData"]["Buses"].size())); i++) //läsa max 6 rader "bussar"
        {
            std::cout << "\tBus number : " << i + 1;
            std::cout << "\t\tDestination: " <<
                      jsonData["ResponseData"]["Buses"][i]["Destination"].asString()
                      << std::endl;
            std::cout << "\tDeparture: " <<
                      jsonData["ResponseData"]["Buses"][i]["DisplayTime"].asString();
            std::cout << "\t\tLine Number: " <<
                      jsonData["ResponseData"]["Buses"][i]["LineNumber"].asString()
                      << std::endl
                      << std::endl;
        }
    } else
    {
        std::cout << "Could not parse HTTP data as JSON" << std::endl;
        std::cout << "HTTP data was:\n" << buffer.c_str() << std::endl;
    }
}


void cUrlAndJsoncpp::getExchangeRatekData()
{
    std::string uri("https://api.exchangerate-api.com/v4/latest/sek");

    headerAndOtherStuff(uri, "GET");


    Json::Value jsonData;
    Json::Reader jsonReader;
    if (jsonReader.parse(buffer.c_str(), jsonData))
    {
        buffer.clear();
        printf("SEK : 1  =>\tUSD : %.6f;\tEUR : %.6f;\tGBP : %.6f.\n", jsonData["rates"]["USD"].asFloat(),
               jsonData["rates"]["EUR"].asFloat(), jsonData["rates"]["GBP"].asFloat());

        saveUSDToSend = jsonData["rates"]["USD"].asFloat();
        saveEURToSend = jsonData["rates"]["EUR"].asFloat();


    } else
    {
        std::cout << "Could not parse HTTP data as JSON" << std::endl;
        std::cout << "HTTP data was:\n" << buffer.c_str() << std::endl;
    }
}


void cUrlAndJsoncpp::getWeatherData(std::string city)
{
    std::string uri("api.openweathermap.org/data/2.5/weather?q=" + city +
                    "&Appid=b4478304347189b7def4ce62c9bcbced&units=metric");
    headerAndOtherStuff(uri, "GET");

    Json::Value jsonData;
    Json::Reader jsonReader;
    if (jsonReader.parse(buffer.c_str(), jsonData))
    {
        buffer.clear();
        //std::cout<<jsonData.toStyledString()<<std::endl<<std::endl;
        std::string name(jsonData["name"].asString());
        if (name == "Stockholm")
            name = "Sthlm";
        const std::string weatherType(jsonData["weather"][0]["main"].asString());
        const std::size_t temperature(jsonData["main"]["temp"].asUInt64());
        std::cout << "\tcity : " << name;
        std::cout << "\t\tweather : " << weatherType;
        std::cout << "\t\ttemperature : " << temperature << std::endl;
        if (city == "Stockholm")
        {
            saveTempToSendStockholm = temperature;
            saveCityToSendStockholm = city;
        } else if (city == "London")
        {
            saveTempToSendLondon = temperature;
            saveCityToSendLondon = city;
        }
    } else
    {
        std::cout << "Could not parse HTTP data as JSON" << std::endl;
        std::cout << "HTTP data was:\n" << buffer.c_str() << std::endl;
    }
}

void cUrlAndJsoncpp::readMockApi()
{

    //read temp
    std::string uri("http://5cd130b1d4a78300147be59f.mockapi.io/nytemp");
    headerAndOtherStuff(uri, "GET");
    Json::Value jsonData;
    Json::Reader jsonReader;
    if (jsonReader.parse(buffer.c_str(), jsonData))
    {
        buffer.clear();
        //std::cout<<jsonData.toStyledString()<<std::endl<<std::endl;
        for (unsigned int i = 0; i < jsonData.size(); i++)
            std::cout << "sent at : " << jsonData[i]["createdAt"].asString()
                      << "\tStockholm : "
                      << jsonData[i]["tempS"].asString()
                      << "\tLondon : "
                      << jsonData[i]["tempL"].asString()
                      << std::endl;
    } else
    {
        std::cout << "Could not parse HTTP data as JSON" << std::endl;
        std::cout << "HTTP data was:\n" << buffer.c_str() << std::endl;
    }
    //read currency
    std::cout << "\n\n";

    std::string uri2("http://5cd130b1d4a78300147be59f.mockapi.io/currency");
    headerAndOtherStuff(uri2, "GET");
    Json::Value jsonData2;
    Json::Reader jsonReader2;
    if (jsonReader2.parse(buffer.c_str(), jsonData2))
    {
        buffer.clear();
        //std::cout<<jsonData.toStyledString()<<std::endl<<std::endl;
        for (unsigned int i = 0; i < jsonData2.size(); i++)
            printf("sent at : %s\tSEK : 1  =>\tUSD : %.6f;\tEUR : %.6f.\n",
                   jsonData2[i]["createdAt"].asString().c_str(),
                   jsonData2[i]["USD"].asFloat(),
                   jsonData2[i]["EUR"].asFloat());

    } else
    {
        std::cout << "Could not parse HTTP data as JSON" << std::endl;
        std::cout << "HTTP data was:\n" << buffer.c_str() << std::endl;
    }
}

void cUrlAndJsoncpp::sendMockApiTemp()
{
    CURL *curl = curl_easy_init();
    CURLcode res;
    if (curl)
    {
        nlohmann::json j;
        j["tempS"] = saveTempToSendStockholm;
        j["nameS"] = saveCityToSendStockholm;
        j["tempL"] = saveTempToSendLondon;
        j["nameL"] = saveCityToSendLondon;

        std::string jsonStr = j.dump();
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");

        curl_easy_setopt(curl, CURLOPT_URL, "http://5cd130b1d4a78300147be59f.mockapi.io/nytemp");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
        // complete within 20 seconds
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        else
            std::cout << "data have been sent successfully!\n";
        // always cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}


void cUrlAndJsoncpp::sendMockApiCurrency()
{
    CURL *curl = curl_easy_init();
    CURLcode res;
    if (curl)
    {
        nlohmann::json j;
        j["USD"] = saveUSDToSend;
        j["EUR"] = saveEURToSend;


        std::string jsonStr = j.dump();
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");

        curl_easy_setopt(curl, CURLOPT_URL, "http://5cd130b1d4a78300147be59f.mockapi.io/currency");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
        // complete within 20 seconds
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        else
            std::cout << "data have been sent successfully!\n";
        // always cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

size_t cUrlAndJsoncpp::curl_write(void *ptr, size_t size, size_t nmemb)
{
    buffer.append((char *) ptr, size * nmemb);
    return size * nmemb;
}

int cUrlAndJsoncpp::kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if ((ch != EOF) && ((ch == 'p') || (ch == 'P')))
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void cUrlAndJsoncpp::headerAndOtherStuff(std::string uri, std::string request)
{

    CURL *curl = curl_easy_init();//skapa och initialisera curl
    CURLcode res; //svar kod "200 is ok"
    if (curl)
    {

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Accept: application/json"); //MIME
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        curl_easy_setopt(curl, CURLOPT_URL, uri.c_str()); //URI
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request.c_str());//GET, POST, DELETE ...
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); //HEADER
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write); //reading funktion
        // complete within 20 seconds
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L); //time out för att avsluta requsten om det tar lång tid
        // curl_easy_setopt(curl, CURLOPT_USERPWD, "sti_alajam:K8yoWP2AEatn1"); //om lösenord behövs
        res = curl_easy_perform(curl); //execute
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        // always cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);    //fwrite( buffer.c_str(), buffer.length(), sizeof(char), stdout);
    }


}

void cUrlAndJsoncpp::nap(int timeToSleep)
{
    int countDown = 0;
    while ((countDown <= timeToSleep) && (!kbhit()))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        countDown += 150;
    }
}

void cUrlAndJsoncpp::getAllAirports()
{
    std::string uri = "https://aviation-edge.com/v2/public/airportDatabase?key=" + aviationStupidKey;
    headerAndOtherStuff(uri, "GET");

    airport airportToPush;

    Json::Value jsonData;
    Json::Reader jsonReader;
    if (jsonReader.parse(buffer.c_str(), jsonData))
    {

        buffer.clear();
        //std::cout << jsonData[0]["codeIataAirport"].asString();
        //std::cout<<jsonData.toStyledString()<<std::endl<<std::endl;

        for (unsigned int i = 0; i < jsonData.size(); i++)
        {
            airportToPush.airportCode = jsonData[i]["codeIataAirport"].asString();
            airportToPush.airportName = jsonData[i]["nameAirport"].asString();
            airportToPush.country = jsonData[i]["nameCountry"].asString();
            allAirports.push_back(airportToPush);

        }

    } else
    {
        std::cout << "Could not parse HTTP data as JSON" << std::endl;
        std::cout << "HTTP data was:\n" << buffer.c_str() << std::endl;
    }
}

void cUrlAndJsoncpp::getArlandaDeparture()
{
    std::string uri(
            "http://aviation-edge.com/v2/public/timetable?key=" + aviationStupidKey + "&iataCode=ARN&type=departure");
    headerAndOtherStuff(uri, "GET");
    Json::Value jsonData;
    Json::Reader jsonReader;
    if (jsonReader.parse(buffer.c_str(), jsonData))
    {
        buffer.clear();
        std::string estimatedTime, terminal, iataCode, realIataCode, country;
        bool found{false};

        for (unsigned int i = 0; i < 6; i++)
        {
            iataCode = jsonData[i]["arrival"]["iataCode"].asString();
            estimatedTime = jsonData[i]["departure"]["estimatedTime"].asString();
            terminal = jsonData[i]["departure"]["terminal"].asString();

            for (auto n: allAirports)
            {
                if (iataCode == n.airportCode)
                {
                    realIataCode = n.airportName;
                    country = n.country;
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                std::cout << "Airport code not found \n\a";
                realIataCode = iataCode;
                country = iataCode;
            }

            std::cout << "\tFlight to : " << realIataCode << "," << country << "\t\tFrom teminal : " << terminal
                      << "\t\tEstimated time : "
                      << estimatedTime
                      << std::endl;
        }
    } else
    {
        std::cout << "Could not parse HTTP data as JSON" << std::endl;
        std::cout << "HTTP data was:\n" << buffer.c_str() << std::endl;
    }

}
