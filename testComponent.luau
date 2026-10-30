
component = {
    name = "MyLuaComponent",
    value = 0,
    on_update = function(self)
        print("Test component running")
        local attachment = self.get_attachment()
        if not attachment:has_component("TestComponentTwo") then
            attachment:add_component("TestComponentTwo")
            print("added component")
        else
            print("We already have TestComponentTwo!!!")
        end
    end,
    on_attach = function(self)
        print("attached")
    end,

    on_deattach = function(self)
        print("GONE")
    end,
}