#include "contact_identifier.h"
#include "contact_state.h"

namespace ca {

Identifier::Identifier(bt::Blackboard& bb)
    : bb_(bb) {}

void Identifier::init() 
{
    contact_event_ = ContactEvent::None;
    contact_mode_ = ContactMode::None;
    // bb_.set("current_contact_event",ContactEvent::None); // TODO: enum class -> string
    // bb_.set("current_contact_mode", ContactMode::None);
    bb_.set("is_contact", 0);
}

void Identifier::update() 
{
    EventIdentifier();
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
    // update previous
    prev_event_ = cur_event_;
    prev_mode_ = cur_mode_;

    // event to str
    if (contact_event_ == ContactEvent::None)
    {
        cur_event_ = "none";
    } 
    else if (contact_event_ == ContactEvent::Impact) 
    {
        cur_event_ = "impact";
    }

    // mode to str
    if (contact_mode_ == ContactMode::None)
    {
        cur_mode_ = "none";
    }
    else if (contact_mode_ == ContactMode::Static) 
    {
        cur_mode_ = "static";
    }
}

} // namespace ca
