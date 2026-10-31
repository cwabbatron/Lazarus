
component = {
    name = "MyLuaComponent",
    value = 0,
    on_update = function(self)
        print("Test component running")
        local attachment = self.get_attachment()
        
        if attachment:has_component("NameComponent") then
            print("removed")
            attachment:remove_component("NameComponent")
        else
            print("did not remove")
            
        end
    end,
    on_attach = function(self)
        print("attached")
    end,

    on_deattach = function(self)
        print("GONE")
    end,
}