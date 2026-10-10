--[[
    GLOBALS ARE:
        
        type entity -> {
            get_components
            
        }
]]

component = {
    name = "MyLuaComponent",
    value = 0,
    on_update = function(self, attachment)
        print("67 test work 100% no virus \n")
        print(`i am {self.name} with value {self.value}\n`)
        self.value += 1
    end,
    on_attach = function(self,attachment)
        print("attached")
    end,

    on_deattach = function(self,attachment)
        print("GONE")
    end,
}