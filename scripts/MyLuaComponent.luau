
component = {
    name = "MyLuaComponent",
    value = 0,
    on_update = function(self)
        print("MyLuaComponent update")
        local attachment = self.get_attachment()

        if attachment:has_component_lua("SecondaryComponent") then
            attachment:remove_component_lua("SecondaryComponent");
            print("removed")
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