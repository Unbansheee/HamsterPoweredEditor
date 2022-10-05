#pragma once
#include <cstdint>
#include <string>

class HPUUID
{
public:
    HPUUID();
    void Generate();
    const std::string& Get() const { return id; }


    bool operator==(const HPUUID& other) const { return id == other.id; }
    bool operator!=(const HPUUID& other) const { return id != other.id; }
    
private:
    friend class Scene;
    friend class Actor;
    std::string id;
    void SetID(const std::string& id) { this->id = id; }
};
