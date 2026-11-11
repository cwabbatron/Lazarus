
component = {
    name = "MyLuaComponent",
    id = 0,
    on_update = function(self)
        print("MyLuaComponent update")
        

        
            
        local entity = EntityManager.add_entity()
            
        self.id = entity.get_id()
        
        entity.add_component_cpp("NameComponent")

        print("Checking component add on created entity.")
        if not entity.has_component_cpp("NameComponent") then
            print("Failed! we dont have NameComponent")
        else
            print("SUCCESS! we have NameComponent")
        end
    end,
    on_attach = function(self)
        print("attached")
    end,

    on_deattach = function(self)
        print("GONE")
    end,
}