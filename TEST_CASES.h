
#ifndef TEST_CASES_h
#define TEST_CASES_h
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
//Main() е докуметиран за да работят тестовете, като го разкоментираме ще и махнем тстовете ще може да се използва интеративния конзолен режим!
//Просто не се сетих него как да го вкарам в тестовете!
TEST_CASE("Loading and saving from file and saving in on existing file!")
{
    SUBCASE("Loading and saving from file and saving in on existing file!")
    {
        Monarch Kingdom{};
            Kingdom.readFromFile("file.txt", Kingdom);
        Kingdom.saveDataToFile(Kingdom, "file_output.txt");
        CHECK(Kingdom.findTreeDepth(Kingdom.GetRoot()) != 0);
        CHECK(std::filesystem::exists("file_output.txt"));
    }
    SUBCASE("Adding member")
    {
        Monarch Kingdom{};
        smart_ptr obj1 = std::make_shared<Node>("Peter Vaklinov", "13.08.1970", 1 , 0 , nullptr);
        Kingdom.AddMember(Kingdom, obj1);
        SUBCASE("Finding first member which is king!")
        {
            CHECK(Kingdom.FindMember(Kingdom.GetRoot(), obj1->Name) == true);
        }
        SUBCASE("Finding all added members!")
        {
            smart_ptr obj2 = std::make_shared<Node>("Antonia Vaklinova", "30.11.2001", 0 , 0 , obj1);
            Kingdom.AddMember(Kingdom, obj2);
            smart_ptr obj3 = std::make_shared<Node>("Stelian Vaklinov", "12.04.1999", 1 , 0 , obj1);
            Kingdom.AddMember(Kingdom, obj3);
            smart_ptr obj4 = std::make_shared<Node>("Daniel Minchev", "27.07.2025", 1 , 1 , obj2);
            Kingdom.AddMember(Kingdom, obj4);
            smart_ptr obj5 = std::make_shared<Node>("Pavla Vaklinova", "17.05.2026", 0 , 0 , obj3);
            Kingdom.AddMember(Kingdom, obj5);
            smart_ptr obj6 = std::make_shared<Node>("Ivan Minchev", "25.01.2002", 1 , 1 , obj1);
            Kingdom.AddMember(Kingdom, obj6);
            CHECK(Kingdom.FindMember(Kingdom.GetRoot(), obj1->Name) == true);
            CHECK(Kingdom.FindMember(Kingdom.GetRoot(), obj2->Name) == true);
            CHECK(Kingdom.FindMember(Kingdom.GetRoot(), obj3->Name) == true);
            CHECK(Kingdom.FindMember(Kingdom.GetRoot(), obj4->Name) == true);
            CHECK(Kingdom.FindMember(Kingdom.GetRoot(), obj5->Name) == true);
            CHECK(Kingdom.FindMember(Kingdom.GetRoot(), obj6->Name) == true);
        }
    }
    SUBCASE("KingChange")
    {
        Monarch Kingdom{};
        smart_ptr obj1 = std::make_shared<Node>("Peter Vaklinov", "13.08.1970", 1 , 0 , nullptr);
        Kingdom.AddMember(Kingdom, obj1);
        smart_ptr obj2 = std::make_shared<Node>("Antonia Vaklinova", "30.11.2001", 0 , 0 , obj1);
        Kingdom.AddMember(Kingdom, obj2);
        smart_ptr obj3 = std::make_shared<Node>("Stelian Vaklinov", "12.04.1999", 1 , 0 , obj1);
        Kingdom.AddMember(Kingdom, obj3);
        smart_ptr obj4 = std::make_shared<Node>("Daniel Minchev", "27.07.2025", 1 , 1 , obj2);
        Kingdom.AddMember(Kingdom, obj4);
        smart_ptr obj5 = std::make_shared<Node>("Pavla Vaklinova", "17.05.2026", 0 , 0 , obj3);
        Kingdom.AddMember(Kingdom, obj5);
        smart_ptr obj6 = std::make_shared<Node>("Ivan Minchev", "25.01.2002", 1 , 1 , obj1);
        Kingdom.AddMember(Kingdom, obj6);
        
        Monarch newKingdom{Kingdom.KingChanger(Kingdom, obj2)};
        CHECK(newKingdom.GetRoot()->Name == "Antonia Vaklinova");
        CHECK(newKingdom.GetRoot()->Religion == 0);
    }
    SUBCASE("HeirsList")
    {
        Monarch Kingdom{};
        smart_ptr obj1 = std::make_shared<Node>("Peter Vaklinov", "13.08.1970", 1 , 0 , nullptr);
        Kingdom.AddMember(Kingdom, obj1);
        smart_ptr obj2 = std::make_shared<Node>("Antonia Vaklinova", "30.11.2001", 0 , 0 , obj1);
        Kingdom.AddMember(Kingdom, obj2);
        smart_ptr obj3 = std::make_shared<Node>("Stelian Vaklinov", "12.04.1999", 1 , 0 , obj1);
        Kingdom.AddMember(Kingdom, obj3);
        smart_ptr obj4 = std::make_shared<Node>("Daniel Minchev", "27.07.2025", 1 , 1 , obj2);
        Kingdom.AddMember(Kingdom, obj4);
        smart_ptr obj5 = std::make_shared<Node>("Pavla Vaklinova", "17.05.2026", 0 , 0 , obj3);
        Kingdom.AddMember(Kingdom, obj5);
        smart_ptr obj6 = std::make_shared<Node>("Ivan Minchev", "25.01.2002", 1 , 1 , obj1);
        Kingdom.AddMember(Kingdom, obj6);
        
        std::list<smart_ptr> heirs = Kingdom.FindHeirs(Kingdom, 2);
        std::string heir_string_names{};
        std::list<smart_ptr>::iterator itr {};
        for(itr = heirs.begin(); itr != heirs.end(); ++itr)
        {
            heir_string_names += (*itr)->Name;
        }
        CHECK(heirs.size() == 2);
        CHECK(heir_string_names == "Stelian VaklinovAntonia Vaklinova");
        
    }
}

#endif 
