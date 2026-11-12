// Shiny Counter - plugin .3gx para Luma3DS (CTRPluginFramework)
// Dibuja un contador en la pantalla inferior y lo guarda en la SD.

#include <3ds.h>
#include <CTRPluginFramework.hpp>
#include <string>
#include <cstdlib>

namespace CTRPluginFramework
{
    // ================== CONFIGURACIÓN ==================
    // Botones que se mantienen pulsados + botón que dispara la acción.
    // Combinaciones separadas del reinicio (L+R+Start) para no interferir.
    static const u32 MODIFICADOR = Key::L;
    static const u32 BTN_SUMAR   = Key::DPadUp;     // L + Arriba    -> +1
    static const u32 BTN_RESTAR  = Key::DPadDown;   // L + Abajo     -> -1
    static const u32 BTN_OCULTAR = Key::DPadLeft;   // L + Izquierda -> mostrar/ocultar
    // Nota: el menú del plugin se abre con Select.

    static const char *RUTA_GUARDADO = "/shiny_counter.txt"; // raíz de la SD
    static const u32 POS_X = 5;
    static const u32 POS_Y = 5;
    // ===================================================

    static u32  g_count   = 0;
    static bool g_visible = true;
    static volatile bool g_saliendo = false;   // true mientras el juego se cierra/reinicia
    static std::string g_texto = "Encuentros: 0"; // texto ya preparado para el overlay

    static void ActualizarTexto()
    {
        g_texto = "Encuentros: " + std::to_string(g_count);
    }

    static void Cargar()
    {
        File f;
        if (File::Open(f, RUTA_GUARDADO, File::READ) != 0)
            return;

        char buf[16] = {0};
        u32 size = static_cast<u32>(f.GetSize());
        if (size > sizeof(buf) - 1)
            size = sizeof(buf) - 1;
        f.Read(buf, size);
        f.Close();

        g_count = static_cast<u32>(std::strtoul(buf, nullptr, 10));
    }

    static void Guardar()
    {
        File f;
        if (File::Open(f, RUTA_GUARDADO, File::WRITE | File::CREATE | File::TRUNCATE) != 0)
            return;

        std::string s = std::to_string(g_count);
        f.Write(s.c_str(), s.size());
        f.Close();
    }

    // Se llama en cada frame: lee los botones.
    static void CadaFrame(Time)
    {
        if (g_saliendo || !Controller::IsKeysDown(MODIFICADOR))
            return;

        if (Controller::IsKeyPressed(static_cast<Key>(BTN_SUMAR)))
        {
            g_count++;
            ActualizarTexto();
            Guardar();
        }
        else if (Controller::IsKeyPressed(static_cast<Key>(BTN_RESTAR)))
        {
            if (g_count > 0)
                g_count--;
            ActualizarTexto();
            Guardar();
        }
        else if (Controller::IsKeyPressed(static_cast<Key>(BTN_OCULTAR)))
        {
            g_visible = !g_visible;
        }
    }

    // Overlay: dibuja el número en la pantalla inferior.
    static bool DibujarContador(const Screen &screen)
    {
        // No se crean strings aquí: esta función corre en cada frame,
        // también mientras el juego se está reiniciando.
        if (g_saliendo || !g_visible || screen.IsTop)
            return false;

        screen.Draw(g_texto, POS_X, POS_Y, Color::White, Color::Black);
        return true;
    }

    void PatchProcess(FwkSettings &settings) { }

    // El juego se cierra (p. ej. reinicio con L+R+Start). El contador ya se
    // guardó al pulsar el botón, así que aquí solo paramos el overlay.
    void OnProcessExit(void)
    {
        g_saliendo = true;
        OSD::Stop(DibujarContador);
    }

    int main(void)
    {
        PluginMenu *menu = new PluginMenu("Shiny Counter", 1, 0, 0,
            "Contador superpuesto para shiny hunting.\n"
            "L+Arriba: +1   L+Abajo: -1\n"
            "L+Izquierda: ocultar   Select: este menu");

        menu->SynchronizeWithFrame(true);
        menu->ShowWelcomeMessage(false);

        Cargar();
        ActualizarTexto();
        OSD::Run(DibujarContador);
        menu->OnNewFrame = CadaFrame;

        // Opciones dentro del menú del plugin (se abre con Select)
        menu->Append(new MenuEntry("Fijar valor", nullptr, [](MenuEntry *)
        {
            Keyboard kb("Nuevo valor del contador:");
            u32 valor = g_count;
            if (kb.Open(valor, g_count) >= 0)
            {
                g_count = valor;
                ActualizarTexto();
                Guardar();
            }
        }));

        menu->Append(new MenuEntry("Poner a 0", nullptr, [](MenuEntry *)
        {
            if (MessageBox("¿Poner el contador a 0?", DialogType::DialogYesNo)())
            {
                g_count = 0;
                ActualizarTexto();
                Guardar();
            }
        }));

        menu->Run();
        delete menu;
        return 0;
    }
}
