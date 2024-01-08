#ifndef TreeDeclaration_h
#define TreeDeclaration_h
#include <string>
#include <list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <functional>
#include <any>
#include <unordered_map>

struct Node;
using smart_ptr = std::shared_ptr<Node>;
static size_t MembersCount = 0;

struct Node
{
    std::string Name{};
    std::string Data_of_born{};
    bool Gender{}; // 1 for male, 0 for female
    bool Religion{}; // 1 for Catholic, 0 for other
    std::list<smart_ptr> Children{};
    std::weak_ptr<Node> Parent{};
    
    Node() = default;
    explicit Node(smart_ptr);
    explicit Node(std::string , std::string , bool , bool ,smart_ptr, std::list<smart_ptr>);
    
};

struct NaturallyDead
{
    std::unordered_set<std::string> abstract_grave{};
    
    bool ValidCorpse(const smart_ptr&) const;
    void AddCorpse(NaturallyDead& , std::string&);
    
};

class Monarch
{
public:
    Monarch();
    
    
    
    
    const smart_ptr& FindMemberR(smart_ptr&, const std::string&);
    bool FindMember(const smart_ptr&, const std::string&); 
    void AddMember(Monarch&, smart_ptr&); 
    template<class T>
    void chageMember(Monarch&, std::string& ,const T&);
    
    
    
    std::list<smart_ptr> FindHeirs (const Monarch&, size_t n);
    
    
    Monarch KingChanger(const Monarch&, smart_ptr&);
    
    
    void readFromFile(const std::string&, Monarch&);
    void saveDataToFile(const Monarch&, const std::string&);
    
    
    
    
    smart_ptr& GetRoot();
    NaturallyDead& GetGrave();
    size_t findTreeDepth(const smart_ptr&) const;
    
private:
   
    smart_ptr root{};
    NaturallyDead grave{};
};







#endif
