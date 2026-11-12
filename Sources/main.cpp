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
    // L+R+Start coincide con el reinicio de Sol/Luna: cada reset suma 1.
    static const u32 MODIFICADOR = Key::L | Key::R;
    static const u32 BTN_SUMAR   = Key::Start;      // L + R + Start     -> +1
    static const u32 BTN_RESTAR  = Key::Select;     // L + R + Select    -> -1
    static const u32 BTN_OCULTAR = Key::DPadLeft;   // L + R + Izquierda -> mostrar/ocultar

    // El menú del plugin se abre con Select por defecto, lo que chocaría
    // con L+R+Select. Lo movemos a L + R + Abajo.
    static const u32 TECLA_MENU  = Key::L | Key::R | Key::DPadDown;

    static const char *RUTA_GUARDADO = "/shiny_counter.txt"; // raíz de la SD
    static const u32 POS_X = 5;
    static const u32 POS_Y = 5;
    // ===================================================

    static u32  g_count   = 0;
    static bool g_visible = true;

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
        if (!Controller::IsKeysDown(MODIFICADOR))
            return;

        if (Controller::IsKeyPressed(static_cast<Key>(BTN_SUMAR)))
        {
            g_count++;
            Guardar();
        }
        else if (Controller::IsKeyPressed(static_cast<Key>(BTN_RESTAR)))
        {
            if (g_count > 0)
                g_count--;
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
        if (!g_visible || screen.IsTop)
            return false;

        screen.Draw("Encuentros: " + std::to_string(g_count), POS_X, POS_Y,
                    Color::White, Color::Black);
        return true;
    }

    void PatchProcess(FwkSettings &settings)
    {
        settings.MenuHotkeys = TECLA_MENU;
    }

    void OnProcessExit(void)
    {
        Guardar();
    }

    int main(void)
    {
        PluginMenu *menu = new PluginMenu("Shiny Counter", 1, 0, 0,
            "Contador superpuesto para shiny hunting.\n"
            "L+R+Start: +1   L+R+Select: -1\n"
            "L+R+Izquierda: ocultar   L+R+Abajo: este menu");

        menu->SynchronizeWithFrame(true);
        menu->ShowWelcomeMessage(false);

        Cargar();
        OSD::Run(DibujarContador);
        menu->OnNewFrame = CadaFrame;

        // Opciones dentro del menú del plugin (se abre con L+R+Abajo)
        menu->Append(new MenuEntry("Fijar valor", nullptr, [](MenuEntry *)
        {
            Keyboard kb("Nuevo valor del contador:");
            u32 valor = g_count;
            if (kb.Open(valor, g_count) >= 0)
            {
                g_count = valor;
                Guardar();
            }
        }));

        menu->Append(new MenuEntry("Poner a 0", nullptr, [](MenuEntry *)
        {
            if (MessageBox("¿Poner el contador a 0?", DialogType::DialogYesNo)())
            {
                g_count = 0;
                Guardar();
            }
        }));

        menu->Run();
        delete menu;
        return 0;
    }
}
