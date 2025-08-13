import re
import os
import shutil
from tkinter import filedialog, Tk

def select_project_folder():
    """
    Kullanıcının bir proje klasörü seçmesi için bir dialog penceresi açar.
    """
    root = Tk()
    root.withdraw()  # Ana tkinter penceresini gizle
    folder_selected = filedialog.askdirectory(title="Lütfen .ino dosyalarınızın bulunduğu ana klasörü seçin")
    return folder_selected

def update_animation_list(base_folder):
    """
    'anim_bin' klasöründeki alt klasörleri tarar ve 'animation_list.txt'
    dosyasını günceller.
    """
    anim_bin_path = os.path.join(base_folder, "anim_bin")
    list_file_path = os.path.join(anim_bin_path, "animation_list.txt")

    print("\n--- Animasyon Listesi Guncelleniyor ---")

    try:
        # anim_bin içindeki tüm alt klasörleri al
        available_animations = [d for d in os.listdir(anim_bin_path) if os.path.isdir(os.path.join(anim_bin_path, d))]
        
        existing_animations = set()
        # Dosya varsa, mevcut animasyonları oku
        if os.path.exists(list_file_path):
            with open(list_file_path, 'r', encoding='utf-8') as f:
                existing_animations = set(line.strip() for line in f)

        # Dosyayı ekleme modunda aç (yoksa oluşturur)
        with open(list_file_path, 'a', encoding='utf-8') as f:
            new_animations_added = 0
            for anim_name in available_animations:
                if anim_name not in existing_animations:
                    f.write(anim_name + '\n')
                    print(f"Listeye eklendi: {anim_name}")
                    new_animations_added += 1
        
        if new_animations_added == 0:
            print("Yeni animasyon bulunmadi, liste guncel.")
        else:
            print(f"{new_animations_added} adet yeni animasyon listeye eklendi.")

    except Exception as e:
        print(f"HATA: Animasyon listesi guncellenirken bir hata olustu: {e}")


def batch_extract_animations():
    """
    Kullanıcı tarafından seçilen bir klasördeki tüm Arduino .ino dosyalarını tarar.
    İçlerindeki PROGMEM animasyon dizilerini ayıklar, her bir kareyi 'anim_bin'
    altındaki kendi klasörüne kaydeder ve işlenen .ino dosyasını
    'anim_conved' klasörüne taşır.
    """
    project_path = select_project_folder()

    if not project_path:
        print("Klasör seçilmedi. İşlem iptal edildi.")
        return
        
    print(f"Ana klasör olarak '{project_path}' seçildi.")

    input_folder = project_path
    output_folder_base = os.path.join(project_path, "anim_bin")
    processed_folder = os.path.join(project_path, "anim_conved")

    print("--- Toplu Animasyon Ayiklayici Baslatildi ---")

    for folder in [output_folder_base, processed_folder]:
        if not os.path.exists(folder):
            os.makedirs(folder)
            print(f"'{folder}' klasoru olusturuldu.")

    ino_files = [f for f in os.listdir(input_folder) if f.endswith('.ino')]

    if not ino_files:
        print(f"\nUYARI: '{input_folder}' klasorunde islenecek .ino dosyasi bulunamadi.")
    else:
        print(f"\nToplam {len(ino_files)} adet .ino dosyasi bulundu. Islem baslatiliyor...")
        for filename in ino_files:
            input_filepath = os.path.join(input_folder, filename)
            print(f"\n--- Isleniyor: {filename} ---")

            try:
                with open(input_filepath, 'r', encoding='utf-8') as f:
                    content = f.read()
            except Exception as e:
                print(f"HATA: Dosya okunurken bir hata olustu: {e}")
                continue

            match = re.search(r'const\s+byte\s+PROGMEM\s+frames\[\]\[\d+\]\s*=\s*\{([\s\S]*?)\};', content)

            if not match:
                print("HATA: Kod icinde 'const byte PROGMEM frames[][...] = {...};' formatinda bir dizi bulunamadi. Bu dosya atlanıyor.")
                continue

            frames_data_string = match.group(1)
            frame_matches = re.findall(r'\{([^{}]*)\}', frames_data_string)

            if not frame_matches:
                print("HATA: Ana dizi icinde animasyon kareleri ('{...}') bulunamadi. Bu dosya atlanıyor.")
                continue

            base_name = os.path.splitext(filename)[0]
            animation_specific_folder = os.path.join(output_folder_base, base_name)
            if not os.path.exists(animation_specific_folder):
                os.makedirs(animation_specific_folder)

            frame_count = 0
            success = True
            for i, frame_str in enumerate(frame_matches):
                try:
                    byte_values = [int(x.strip()) for x in frame_str.split(',') if x.strip()]
                    frame_byte_array = bytearray(byte_values)

                    if len(frame_byte_array) != 512:
                        print(f"UYARI: {i}. kare beklenen 512 byte boyutunda degil (Boyut: {len(frame_byte_array)} byte). Atlanıyor.")
                        continue

                    output_filename = os.path.join(animation_specific_folder, f"anim_{i}.bin")

                    with open(output_filename, 'wb') as f_out:
                        f_out.write(frame_byte_array)
                    
                    frame_count += 1
                except Exception as e:
                    print(f"HATA: {i}. kare islenirken hata olustu: {e}")
                    success = False
                    break
            
            if success:
                print(f"Basarili: {frame_count} adet kare '{animation_specific_folder}' klasorune kaydedildi.")
                try:
                    shutil.move(input_filepath, os.path.join(processed_folder, filename))
                    print(f"'{filename}' dosyasi '{processed_folder}' klasorune tasindi.")
                except Exception as e:
                    print(f"HATA: Dosya tasinirken hata olustu: {e}")
            else:
                print(f"'{filename}' dosyasinda hata olustugu icin islem yarida kaldi ve dosya tasinmadi.")

    # Tüm .ino işlemleri bittikten sonra animasyon listesini güncelle
    update_animation_list(project_path)

    print("\n--- TUM ISLEMLER TAMAMLANDI ---")

if __name__ == "__main__":
    batch_extract_animations()
