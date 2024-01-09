#include <iostream>
#include <queue>
#include "TreeDeclaration.h"
#include "TEST_CASES.h"

//Main() е докуметиран за да работят тестовете, като го разкоментираме ще и махнем тстовете ще може да се използва интеративния конзолен режим!
//Просто не се сетих него как да го вкарам в тестовете!
Monarch::Monarch():root{std::make_shared<Node>()}{}
Node::Node(smart_ptr other)
{
    Name = other->Name;
    Data_of_born = other->Data_of_born;
    Gender = other->Gender;
    Religion = other->Religion;
    Children = other->Children;
    Parent = other->Parent;
}

Node::Node(std::string _name , std::string _data , bool _g , bool _r ,smart_ptr _p,  std::list<smart_ptr> _nul = {} ): Name{_name}, Data_of_born{_data}, Gender{_g}, Religion{_r} , Parent{_p}{}


bool NaturallyDead::ValidCorpse(const smart_ptr& member) const
{
    if(member->Religion == 1)
    {
        return true;
    }
    return false;
}

void NaturallyDead::AddCorpse(NaturallyDead& grave, std::string& name)
{
    grave.abstract_grave.insert(name);
}
const smart_ptr& Monarch::FindMemberR(smart_ptr& root_of_kingdom, const std::string& name)
{
    if (root_of_kingdom->Name == name) {
        return root_of_kingdom;
    }
    
    for (smart_ptr& child : root_of_kingdom->Children)
    {
        // Recursively search in the children
        const smart_ptr& result = FindMemberR(child, name);
        
        // Check if the result is found in the current subtree
        if (result != nullptr && result->Name == name) {
            return result;
        }
    }
    // If the target name is not found in this subtree, return nullptr
    return nullptr;
}

bool Monarch::FindMember(const smart_ptr& root_of_kingdom, const std::string& name)
{
    if (root_of_kingdom == nullptr)
    {
        return false;
    }

    if (root_of_kingdom->Name == name) 
    {
        return true;  // Value found
    }

    std::list<smart_ptr>::iterator itr {};
    for(itr = root_of_kingdom->Children.begin(); itr !=root_of_kingdom->Children.end(); ++itr)
    {
        if (FindMember((*itr), name))
        {
            return true;  // Value found in one of the children
        }
    }
    return false;
}

void Monarch::AddMember(Monarch& kingdom , smart_ptr& member)
{
    
    smart_ptr share_helper = member->Parent.lock();
    if(share_helper == nullptr)
    {
        std::cout<<"AddMember(smart_ptr&)PARENT"<<std::endl;
        if(kingdom.grave.ValidCorpse(member))
        {
            kingdom.grave.AddCorpse(kingdom.grave, member->Name);
        }
        MembersCount++;
        kingdom.root = member;
    }
    else
    {
        if(FindMember(kingdom.root, share_helper->Name))
        {
            std::cout<<"AddMember(smart_ptr&)"<<std::endl;
            if(kingdom.grave.ValidCorpse(member))
            {
                kingdom.grave.AddCorpse(kingdom.grave, member->Name);
            }
            MembersCount++;
            share_helper->Children.push_back(member);
        }
        else
        {
            std::cout<<"Error adding member"<<std::endl;
        }
    }
}

template<class T> void Monarch::chageMember(Monarch& kingdom, std::string& name_of_seching_member , const T& new_data)
{
    
    //Всеки член може да си променя името или религията.
    if(!FindMember(kingdom.root, name_of_seching_member))
    {
        std::cerr<<"Error finding member"<<std::endl;
        return;
    }
    
    
    if(std::is_same<T, std::string>::value == true)
    {
        FindMemberR(kingdom.root, name_of_seching_member)->Name = new_data;
    }
    else if(std::is_same<T, bool>::value == true)
    {
        FindMemberR(kingdom.root, name_of_seching_member)->Religion = new_data;
        if(new_data == 1)
        {
            kingdom.grave.AddCorpse(kingdom.grave, name_of_seching_member);
        }
    }
}


std::list<smart_ptr> Monarch::FindHeirs(const Monarch& kingdom, size_t n) 
{
    std::list<smart_ptr> heirsList = {};
    if(n > (MembersCount - kingdom.grave.abstract_grave.size()))
    {
        return heirsList; //връщаме празен списък.
    }
        
    std::queue<smart_ptr> q{};
    q.push(kingdom.root);

    while (!q.empty()) {
        size_t levelSize = q.size();
        for (int i = 0; i < levelSize; ++i)
        {
            smart_ptr current = q.front();
            q.pop();
            smart_ptr helper = current->Parent.lock();

            // Process the current node
            if(kingdom.grave.abstract_grave.find(current->Name) == kingdom.grave.abstract_grave.end())
            {
                    heirsList.push_back(current);
                    // Enqueue children of the current node
                    for (const smart_ptr& child : current->Children) {
                        if (child)
                            q.push(child);
                    }
            }
        }
    }
    
    heirsList.pop_front();
    
    //функция която да сортира heirsList по големина на датата за раждане,като я превърнем в число от стринг.
    bool swapped;
    do {
        swapped = false;
        for (auto it = heirsList.begin(); it != std::prev(heirsList.end()); ++it) {
            auto next = std::next(it);
            
            (*it)->Data_of_born.erase(std::remove((*it)->Data_of_born.begin(), (*it)->Data_of_born.end(), '.'), (*it)->Data_of_born.end());
            (*next)->Data_of_born.erase(std::remove((*next)->Data_of_born.begin(), (*next)->Data_of_born.end(), '.'), (*next)->Data_of_born.end());
            
            if ((std::stoi((*it)->Data_of_born)%10000) > (std::stoi((*next)->Data_of_born)%10000))
            {
                std::swap(*it, *next);
                swapped = true;
            }
            else if((std::stoi((*it)->Data_of_born)%10000) == (std::stoi((*next)->Data_of_born)%10000))
            {
                if (((std::stoi((*it)->Data_of_born)%1000000)*100) > ((std::stoi((*next)->Data_of_born)%1000000)*100))
                {
                    std::swap(*it, *next);
                    swapped = true;
                }
                else if(((std::stoi((*it)->Data_of_born)%1000000)*100) == ((std::stoi((*next)->Data_of_born)%1000000)*100))
                {
                    if (((std::stoi((*it)->Data_of_born)%100000000)*100) > ((std::stoi((*next)->Data_of_born)%100000000)*100))
                    {
                        std::swap(*it, *next);
                        swapped = true;
                    }
                }
            }
        }
    } while (swapped);
    //След това ичтриваме елементие след n и връщаме.
    size_t count = heirsList.size() - n;
    while(count != 0)
    {
        heirsList.pop_back();
        count--;
    }
    return heirsList;
}
Monarch Monarch::KingChanger(const Monarch& kingdom,smart_ptr& newK_Q)
{
    if(!FindMember(kingdom.root, newK_Q->Name))
    {
        std::cerr<<"we do not find this member"<<std::endl;
        return kingdom;
    }
    
    Monarch newKingdom{};
    MembersCount = 0;
    smart_ptr share_helper = newK_Q->Parent.lock();
    if(newKingdom.grave.ValidCorpse(newK_Q))
    {
        newKingdom.grave.AddCorpse(newKingdom.grave, newK_Q->Name);
    }
    MembersCount++;
    std::cout<<"AddMember(smart_ptr&)PARENT"<<std::endl;
    newKingdom.root = newK_Q;
    for(const auto& child : newK_Q->Children)
    {
        if(newKingdom.grave.ValidCorpse(child))
        {
            newKingdom.grave.AddCorpse(newKingdom.grave, child->Name);
        }
        MembersCount++;
        std::cout<<"AddMember(smart_ptr&)"<<std::endl;
        share_helper->Children.push_back(child);
    }
    return newKingdom;
}
void Monarch::readFromFile(const std::string& filename, Monarch& kingdom)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return ;
    }
    std::string line;
    while (std::getline(file,line)) {
        std::istringstream iss(line);
        std::string name{};
        std::string date{};
        bool gender{};
        bool religion{};
        std::string parent{};
        std::string part{};
        
        iss >> name;
        iss>>part;
        name += " " + part;
        iss>>date;
        iss>>gender;
        iss>>religion;
        iss >> parent;
        if(parent.empty())
        {
            smart_ptr member = std::make_shared<Node>(name, date, gender , religion , nullptr);
            kingdom.AddMember(kingdom, member);
        }
        else
        {
            iss>>part;
            parent += " " + part;
            smart_ptr member = std::make_shared<Node>(name, date, gender , religion , kingdom.FindMemberR(kingdom.root, parent));
            kingdom.AddMember(kingdom, member);
        }
    }
    file.close();
}
void Monarch::saveDataToFile(const Monarch& kingdom, const std::string& filename)
{
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) 
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    std::queue<smart_ptr> q{};
    q.push(kingdom.root);

    while (!q.empty()) {
        size_t levelSize = q.size();
        for (int i = 0; i < levelSize; ++i)
        {
            smart_ptr current = q.front();
            q.pop();
            
            // Process the current node
            smart_ptr helper = current->Parent.lock();
            outputFile << current->Name<<" ";
            outputFile << current->Data_of_born<<" ";
            outputFile << current->Gender<<" ";
            outputFile << current->Religion<<" ";
            if(helper == nullptr)
            {
                outputFile<<std::endl;
            }
            else
            {
                outputFile << helper->Name<<std::endl;
            }
            // Enqueue children of the current node
            for (const smart_ptr& child : current->Children) {
                if (child)
                    q.push(child);
            }
        }
    }
    outputFile.close();
}

smart_ptr& Monarch::GetRoot()
{
    return root;
}
NaturallyDead& Monarch::GetGrave()
{
    return grave;
}
size_t Monarch::findTreeDepth(const smart_ptr& root) const
{
    if (root == nullptr) {
            return 0; // Base case: empty tree has depth 0
        }

        size_t maxChildDepth = 0;
        for (auto& child : root->Children) {
            size_t childDepth = findTreeDepth(child);
            maxChildDepth = std::max(maxChildDepth, childDepth);
        }

        return 1 + maxChildDepth;
}
void Interactive_Prompt(Monarch& Kingdom)
{
    std::string userInput;
    while (true)
    {
        // Display the prompt
        std::cout << "> ";
        
        // Get user input
        std::getline(std::cin, userInput);
        
        // Check for exit condition
        if (userInput == "quit") {
            std::cout << "Goodbye!"<<std::endl;
            break;
        }
        
        if(userInput == "add-member")
        {
            std::string name{};
            std::string date{};
            bool gender{};
            bool religion{};
            std::string Pname{};
            std::cout << "You entered: " << userInput <<std::endl;
            if(MembersCount == 0)
            {
                std::cout<<"Enter name, date of birth, gender(1 for male / 0 for woman), religion(1 for catolic / 0 for other)"<<std::endl;
                std::getline(std::cin, name);
                std::cin>>date>>gender>>religion;
                std::cin.ignore();
                smart_ptr member = std::make_shared<Node>(name,date,gender,religion,nullptr);
                Kingdom.AddMember(Kingdom, member);
            }
            else
            {
                std::cout<<"Enter name, date of birth, gender(1 for male / 0 for woman), religion(1 for catolic / 0 for other) and parent name"<<std::endl;
                std::getline(std::cin, name);
                std::cin>>date>>gender>>religion;
                std::cin.ignore();
                std::getline(std::cin, Pname);
                smart_ptr member = std::make_shared<Node>(name,date,gender,religion,Kingdom.FindMemberR(Kingdom.GetRoot(), Pname));
                Kingdom.AddMember(Kingdom, member);
            }
        }
        else if(userInput == "print-info")
        {
            std::cout << "You entered: " << userInput <<std::endl;
            std::string name{};
            std::getline(std::cin, name);
            if(Kingdom.FindMember(Kingdom.GetRoot(), name))
            {
                smart_ptr helper = Kingdom.FindMemberR(Kingdom.GetRoot(), name);
                smart_ptr helper1 = helper->Parent.lock();
                if(helper1 == nullptr)
                {
                    std::cout<<helper->Name<<" "<<helper->Data_of_born<<" "<<helper->Gender<<" "<<helper->Religion<<std::endl;
                }
                else
                {
                    std::cout<<helper->Name<<" "<<helper->Data_of_born<<" "<<helper->Gender<<" "<<helper->Religion<<" "<<helper1->Name<<std::endl;
                }
            }
            else
            {
                std::cout<<"we dont have that member name"<<std::endl;
            }
        }
        else if(userInput == "make-monarch")
        {
            std::cout << "You entered: " << userInput <<std::endl;
            std::string name{};
            std::getline(std::cin, name);
            if(Kingdom.FindMember(Kingdom.GetRoot(), name))
            {
                smart_ptr helper = Kingdom.FindMemberR(Kingdom.GetRoot(), name);
                Monarch newKingdom {Kingdom.KingChanger(Kingdom, helper)};
                Interactive_Prompt(newKingdom);
                return;
            }
            else
            {
                std::cout<<"we dont have that member name"<<std::endl;
            }
        }
        else if(userInput == "edit-religion")
        {
            std::cout << "You entered: " << userInput <<std::endl;
            std::string name{};
            bool religion{};
            std::getline(std::cin, name);
            std::cin>>religion;
            std::cin.ignore();
            if(Kingdom.FindMember(Kingdom.GetRoot(), name))
            {
                Kingdom.chageMember(Kingdom, name, religion);
            }
            else
            {
                std::cout<<"we dont have that member name"<<std::endl;
            }
        }
        else if(userInput == "declare-deceased")
        {
            std::cout << "You entered: " << userInput <<std::endl;
            std::string name{};
            std::getline(std::cin, name);
            if(Kingdom.FindMember(Kingdom.GetRoot(), name))
            {
                Kingdom.GetGrave().AddCorpse(Kingdom.GetGrave(), name);
            }
            else
            {
                std::cout<<"we dont have that member name"<<std::endl;
                
            }
        }
        else if(userInput == "find-successors")
        {
            size_t n{};
            std::cin>>n;
            std::cin.ignore();
            std::list<smart_ptr> heirs = Kingdom.FindHeirs(Kingdom, n);
            if(heirs.empty())
            {
                std::cout<<"number of heirs is not correct, pls enter again"<<std::endl;
            }
            else
            {
                std::list<smart_ptr>::iterator itr {};
                for(itr = heirs.begin(); itr != heirs.end(); ++itr)
                {
                    std::cout<<(*itr)->Name<<std::endl;
                }
            }
        }
        else if(userInput == "save")
        {
            std::string filename{};
            std::getline(std::cin,filename);
            Kingdom.saveDataToFile(Kingdom, filename);
        }
        else if(userInput == "load")
        {
            std::string filename{};
            std::getline(std::cin,filename);
            Kingdom.readFromFile(filename, Kingdom);
        }
    }
}
//int main() {
//    std::cout << "Welcome to the softoware that you can manage royal family!"<<std::endl;
//    Monarch kingdom{};
//    Interactive_Prompt(kingdom);
//        
//    
//    
//    
//
//    return 0;
//}
