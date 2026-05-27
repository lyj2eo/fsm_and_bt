#include "contact_identifier.h"
#include "contact_state.h"

namespace ca {

Identifier::Identifier(bt::Blackboard& bb)
    : bb_(bb) {}

void Identifier::init() 
{
    // bb_.set("current_contact_event",ContactEvent::None); // TODO: enum class -> string
    // bb_.set("current_contact_mode", ContactMode::None);
    bb_.set("is_contact", 0);
}

void Identifier::update() 
{
    isContact();
}

bool Identifier::isContact() const 
{

    bool is_contact = false;
    std::vector<double> fts_force = bb_.get<std::vector<double>>("fts_force");
    
    if(fts_force[2] >= 10.0) 
    {
        is_contact = true;   
    }

    bb_.update("is_contact", is_contact);
    
    return is_contact;
}

void Identifier::EventIdentifier() 
{
    ContactEvent::None;
    
}

} // namespace ca
