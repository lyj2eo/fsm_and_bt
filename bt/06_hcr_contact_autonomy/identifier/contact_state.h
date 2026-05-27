#pragma once

namespace ca {

enum class ContactEvent 
{
    None,
    Touchdown, 
    Stable,         
    ContactLoss,
    Impact,
};

enum class ContactMode 
{
    None,
    Unknown,
    Static,
    Sliding,
    Jamming,
};

} // namespace ca
