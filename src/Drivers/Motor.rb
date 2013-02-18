@class = "4F"
@desc = "Motor Controller"

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
    //DebugPrint(self->id);
    LATAbits.LATA2 = !LATAbits.LATA2;
  }
end
