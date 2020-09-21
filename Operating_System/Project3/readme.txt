혹시나 kernel panic이 mlfqs에서 계속 뜬다면

#define FRACTION 1<<14 에서 괄호를 치도록...

#define FRACTION (1<<14)