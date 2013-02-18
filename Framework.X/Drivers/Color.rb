@class = "20"
@desc = "Color sensing driver"

@members = lambda do
  out %{
    unsigned char a;
  }
end

@init = lambda do
  out %{
    self->a = 0xE;
    DebugPrint(self->a);
  }
end

@respond = lambda do
  out %{
  }
end

@poll = lambda do
  out %{
    LATAbits.LATA0 = !LATAbits.LATA0;
  }
end
