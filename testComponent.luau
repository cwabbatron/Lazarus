--[[
    GLOBALS ARE:
        
        type entity -> {
            get_components
            
        }
]]

component = {
    name = "MyLuaComponent",
    on_update = function(self,attachment)
        print("67 test work 100% no virus")
    end,
    on_attach = function(self,attachment)
        print("attached")
    end,

    on_deattach = function(self,attachment)
        print("GONE")
    end,
}