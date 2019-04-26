# SoalShift_modul4_D10</br>

## Soal 1 </br>
Semua nama file dan folder harus terenkripsi</br>
Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai: </br>

  ``qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0``</br>

Misalkan ada file bernama **“halo”** di dalam folder **“INI_FOLDER”**, dan key yang dipakai adalah 17, maka:</br>
**“INI_FOLDER/halo"** saat belum di-mount maka akan bernama **“n,nsbZ]wio/QBE#”**, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi **“INI_FOLDER/halo”**.</br>
_Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan_</br>
### Penjelasan soal:</br>
soal ini meminta kita untuk mengenkripsi nama file dan folder yang ada pada file system. Enkripsi menggunakan Caesar cipher dengan aturan yang telah ditentukan pada soal dan berdasarkan key yang ditentukan. dekripsi akan dilakukan ketika telah di-mount. karakter '/' akan diabaikan.</br>
### Solusi:</br>
untuk fungsi enkripsi kita akan mencari character yang terdapat pada list Caesar cipher dan mengganti character tersebut dengan character ke 17 setelah character tersebut pada list Caesar cipher list, jika melebihi indeks maka dimulai dari indeks ke 0 lagi. dan ketika menemui '/' maka characternnya tetap dengan cara :</br>
```
char key[100] = {"qE1~ YMUR2\"`hNIdPzi\%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0"};
//enkripsi no1
void enkripsi(char *path){
	int pnjng = strlen(path), i, j;

	for(i=0; i<pnjng; i++){
		if(path[i] == '/'){
			continue;
		}
		for(j=0; j<94; j++){
			if(path[i] == key[j]){
				path[i] = key[(j + 17)%94];
				break;
			}
		}
	}
}
```
untuk fungsi dekripsi kita akan mencari character yang terdapat pada list Caesar cipher dan mengganti character tersebut dengan character ke 17 sebelum character tersebut pada list Caesar cipher list, jika melebihi indeks maka dimulai dari indeks ke 93 lagi. ketika menemui '/' maka characternnya tetap dengan cara :</br>
```
void dekripsi(char *path){
	int pnjng = strlen(path), i, j;
	for(i=0; i<pnjng; i++){
		if(path[i] == '/'){
			continue;
		}
		for(j=0; j<94; j++){
			if(path[i] == key[j]){
				path[i] = key[(94+(j - 17))%94];
				break;
			}
		}
	}
}
```


## Soal 2</br>
Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”</br>
Urutan operasi dari kebutuhan ini adalah:</br>

  1. Tepat saat sebelum file system di-mount</br>
     - Secara otomatis folder “Videos” terbuat di root directory file system</br>
     - Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”, “computer.mkv.001”, “computer.mkv.002”, dst. Maka secara otomatis file pecahan tersebut akan di-join menjadi file video “computer.mkv”</br>
Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa file. File pecahan tersebut dijamin terletak di root folder fuse</br>
     - Karena mungkin file video sangat banyak sehingga mungkin saja saat menggabungkan file video, file system akan membutuhkan waktu yang lama untuk sukses ter-mount. Maka pastikan saat akan menggabungkan file pecahan video, file system akan membuat 1 thread/proses(fork) baru yang dikhususkan untuk menggabungkan file video tersebut</br>
     - Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”</br>
     - Jangan tampilkan file pecahan di direktori manapun</br>
  
  2. Tepat saat file system akan di-unmount</br>
     - Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus file pecahan yang terdapat di root directory file system</br>
     - Hapus folder “Videos” </br>

### Solusi:</br>


## Soal 3 </br>
Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
- Owner Name 	: ‘chipset’ atau ‘ic_controller’
- Group Name	: ‘rusak’
- Tidak dapat dibaca

Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.</br>

### Solusi:</br>


## Soal 4 </br>
Pada folder **YOUTUBER**, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah **“.iz1”**. File berekstensi **“.iz1”** tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”</br>

### Solusi:</br>


## Soal 5 <br>
Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama **Backup** kemudian hasil dari save tersebut akan disimpan pada backup dengan nama **namafile_[timestamp].ekstensi**. Dan ketika file asli dihapus, maka akan dibuat folder bernama **RecycleBin**, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama **namafile_deleted_[timestamp].zip** dan ditaruh kedalam folder RecycleBin (file asli dan backup terhapus). Dengan format **[timestamp]** adalah **yyyy-MM-dd_HH:mm:ss**</br>

### Solusi:</br>



