
component = {
    name = "TestComponent 2",
    value = 0,
    on_update = function(self, attachment)
        print(`TestComponent2 running with value {self.value}`)
        self.value += 2
    end,
    on_attach = function(self,attachment)
        print("attached")
    end,

    on_deattach = function(self,attachment)
        print("GONE")
    end,
}