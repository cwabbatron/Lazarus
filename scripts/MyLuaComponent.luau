
component = {
    name = "MyLuaComponent",
    value = 0,
    on_update = function(self)
        print("MyLuaComponent update")
        local attachment = self.get_attachment()
        if not attachment:has_component_lua("SecondaryComponent") then
            attachment:add_component_lua("SecondaryComponent");
            print("added")
        else
            print("not added")
        end
    end,
    on_attach = function(self)
        print("attached")
    end,

    on_deattach = function(self)
        print("GONE")
    end,
}