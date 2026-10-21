
component = {
    name = "MyLuaComponent",
    value = 0,
    on_update = function(self)
        print("Test component running")
        
        
        local attachment = self.get_attachment()

        print(`id is {attachment.get_id()}`)
        
        print(`do we have nameComponent {attachment.has_component("NameComponent")}`)
        print(`do we have nonExistentComponet? {attachment.has_component("nonExistantComponent")}`)
    end,
    on_attach = function(self)
        print("attached")
    end,

    on_deattach = function(self)
        print("GONE")
    end,
}