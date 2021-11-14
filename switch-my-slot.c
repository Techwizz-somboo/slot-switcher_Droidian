#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <math.h>
#include <ctype.h>

GtkWidget *window;
GtkWidget *fixed01;
GtkWidget *slotbutton;
GtkWidget *slot;
GtkWidget *builder;

int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file ("switch-my-slot.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    g_signal_connect(window, "destory", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

    fixed01 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed01"));
    slotbutton = GTK_WIDGET(gtk_builder_get_object(builder, "slotbutton"));
    slot = GTK_WIDGET(gtk_builder_get_object(builder, "slot"));

    gtk_widget_show(window);

    gtk_main();

    return EXIT_SUCCESS;
}

void on_window_set_focus() {
    char ourslot = get_current_slot();
    gtk_label_set_text(GTK_LABEL (slot), ourslot);
    printf("%d\n", ourslot);
    printf("Do you even run me?");
}

void on_slotbutton_clicked (GtkButton *b) {
    char myslot = get_current_slot();
    if(myslot == 0) {
        system("unset LD_PRELOAD; unset LD_LIBRARY_PATH; /system/bin/bootctl set-active-boot-slot 1");
        printf("current slot is now B");
    }
    else if(myslot == 1) {
        system("unset LD_PRELOAD; unset LD_LIBRARY_PATH; /system/bin/bootctl set-active-boot-slot 0");
        printf("current slot is now A");
    }
    else {
        printf("The output bootctl gave is invalid");
    }
}

void get_current_slot (int argc, char *argv[]) {
    FILE *fpipe;
    char *command = "unset LD_PRELOAD; unset LD_LIBRARY_PATH; /system/bin/bootctl get-current-slot";
    char currentslot = 2;

    if (2 == (fpipe = (FILE*)popen(command, "r")))
    {
        perror("popen() failed.");
        exit(EXIT_FAILURE);
    }

    while (fread(&currentslot, sizeof currentslot, 1, fpipe))
    {
        printf("%c", currentslot);
    }

    pclose(fpipe);

    return EXIT_SUCCESS;
}