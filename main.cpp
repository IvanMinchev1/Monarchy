
#include <iostream>
#include <queue>
#include "TreeDeclaration.h"


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

    return nullptr; // Дава предупрежение, че връщаме temporary като l-value, no преди да върнем имаме проверк, че не може да е nullptr.
}

bool Monarch::FindMember(const smart_ptr& root_of_kingdom, const std::string& name)
{
    if (root_of_kingdom == nullptr)
    {
        return false;
    }

    if (root_of_kingdom->Name == name) 
    {
        return true;  
    }

    std::list<smart_ptr>::iterator itr {};
    for(itr = root_of_kingdom->Children.begin(); itr !=root_of_kingdom->Children.end(); ++itr)
    {
        if (FindMember((*itr), name))
        {
            return true; 
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

            if(kingdom.grave.abstract_grave.find(current->Name) == kingdom.grave.abstract_grave.end())
            {
                    heirsList.push_back(current);
                    for (const smart_ptr& child : current->Children) {
                        if (child)
                            q.push(child);
                    }
            }
        }
    }
    
    heirsList.pop_front();
    
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
int main() {
    Monarch Kingdom{};
    Kingdom.readFromFile("file.txt", Kingdom);
    Kingdom.saveDataToFile(Kingdom, "file_output.txt");
    

    return 0;
}
