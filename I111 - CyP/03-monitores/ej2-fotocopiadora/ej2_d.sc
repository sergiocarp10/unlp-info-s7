/* Modifique la solución de (a) para el caso en que se deba respetar estrictamente el orden 
dado por el identificador del proceso (la persona X no puede usar la fotocopiadora 
hasta que no haya terminado de usarla la persona X-1). */

Monitor Fotocopiadora
{
    int siguiente = 0, restantes = N;
    Cond espera[N];

    Procedure Esperar(idP : in int)
    {
        if (not siguiente = idP)
        {
            esperando++;
            wait(espera[idP]);
        }
    }
    
    Procedure Avanzar()
    {
        if (restantes > 0)
        {
            siguiente = (siguiente + 1) % N;
            signal(espera[siguiente]);
        }
    }
}

Process Persona[id: 0..N-1]
{
    Fotocopiadora.Esperar(id);
    Fotocopiar();
    Fotocopiadora.Avanzar();
}
