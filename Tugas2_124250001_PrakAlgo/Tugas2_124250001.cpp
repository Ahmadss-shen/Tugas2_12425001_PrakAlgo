#include <iostream>
#include <iomanip>
using namespace std;

struct Node {
    int nip;
    string nama;
    string divisi;
    string status;

    Node* left;
    Node* right;

    Node(int pNip, string pNama, string pDivisi){
        nip = pNip;
        nama = pNama;
        divisi = pDivisi;

        // status awal
        status ="Tidak cuti";

        left =NULL;
        right =NULL;
    }
};

/* menyimpan riwayat dri aksi undo */
struct Stack{
    int nip;
    string aksi;

    Stack* next;
};

Stack* top = NULL;

/*menyimpan antrian karyawan yg cuti*/
struct Queue{
    int nip;

    Queue* next;
};

Queue* front = NULL;
Queue* rear = NULL;

Node* root = NULL;

/*Menambahkan data ke dalam stack*/
void push(int nip, string aksi){
    Stack* baru = new Stack;

    baru->nip = nip;
    baru->aksi = aksi;

    baru->next = top;
    top = baru;
}

/*Menghapus data yg teratas*/
void pop() {
    if(top == NULL) {
        return;
    }
    Stack* hapus = top;

    top = top->next;
    delete hapus;
}

/*Menambahkan data*/
void enqueue(int nip){
    Queue* baru = new Queue;

    baru->nip = nip;
    baru->next = NULL;

    // jika dalam queue kosong
    if(front == NULL) {
        front = baru;
        rear = baru;
    } else{
        rear->next = baru;
        rear = baru;
    }
}

/*hapus data paling depan*/
int dequeue(){
    if(front == NULL){
        return -1;
    }
    Queue* hapus = front;
    int nip = hapus->nip;

    front = front->next;
    delete hapus;
    
    return nip;
}

/*Menambahkan data karyawan*/
void insert(Node*& root,int nip,string nama,string divisi){
	
    if(root ==NULL){
        root = new Node(nip, nama, divisi);

        cout <<"\nData karyawan berhasil ditambahkan\n";
        return;
    }
    // masuk yg kiri dan kanan
    if(nip < root->nip){
        insert(root->left,nip,nama,divisi);
    }
    else if(nip > root->nip){
        insert(root->right,nip,nama,divisi);
    }
}

/*cari data berdasarkan nip */
Node* search(Node* root, int nip){
	
    if(root == NULL){
        return NULL;
    }
    if(root->nip == nip){
        return root;
    }
    if(nip < root->nip){
        return search(root->left, nip);
    }
    else {
        return search(root->right, nip);
    }
}

/*menampilkan data secara urut*/
void inorder(Node* root) {
	
    if(root== NULL){
        return;
    }
    inorder(root->left);
    cout <<left;

    cout <<setw(10) << root->nip
         <<setw(25) <<root->nama
         <<setw(20) << root->divisi
         <<setw(20) << root->status
         <<endl;

    inorder(root->right);
}

/*tampilkan karyawannya*/
void tampilKaryawan(){
	
    if(root == NULL){
        cout << "\nData karyawan kosong\n";
        return;
    }
    cout <<"\n==================================================================\n";
    cout <<"|";
    cout<<left;
    cout<< setw(10) <<"NIP"
         << setw(25) <<"Nama"
         << setw(20) << "Divisi"
         << setw(20) <<"Status   |"
         << endl;
    cout <<"==================================================================\n";
    inorder(root);
    cout <<"==================================================================\n";
}

void pengajuanCuti(){
    int nip;

    cout << "\nMasukkan NIP : ";
    cin >> nip;

    Node* karyawan = search(root, nip);

    if(karyawan == NULL){
        cout <<"Karyawan tidak ditemukan!\n";
        return;
    }
    if(karyawan->status =="Tidak Cuti"){
        karyawan->status = "Cuti";

        push(nip, "pengajuan");
        cout <<"Pengajuan cuti berhasil\n";
    } else {
        enqueue(nip);

        cout <<"Karyawan sedang cuti\n";
        cout <<"Pengajuan masuk ke dalam antrian\n";
    }
}

void selesaiCuti() {
    int nip;

    cout<<"\nMasukkan NIP: ";
    cin >> nip;

    Node* karyawan = search(root, nip);

    if(karyawan == NULL){
        cout <<"Karyawan tidak ditemukan!\n";
        return;
    }
    karyawan->status ="Tidak Cuti";

    push(nip, "selesai");
    cout <<"Cuti berhasil diselesaikan\n";

    if(front != NULL){
        int nipBaru = dequeue();

        Node* dataBaru = search(root, nipBaru);

        if(dataBaru != NULL){
        dataBaru->status ="Cuti";
        cout <<"Pengajuan berikutnya diproses dari antrian\n";
        }
    }
}

/* node kecil untuk hapus node*/
Node* cariKecil(Node* root){
	
    while(root->left != NULL){
    root = root->left;
    }
    return root;
}

/* hapus node*/
Node* hapus(Node* root, int nip){
    if(root == NULL){
        return root;
    }
    if(nip < root->nip){
    root->left = hapus(root->left, nip);
    }
    else if(nip > root->nip){
        root->right = hapus(root->right, nip);
    } else {
		
        if(root->left == NULL &&
           root->right == NULL)
        {
            delete root;
            cout <<"Data karyawan berhasil dihapus\n";
            return NULL;
        }
        else if(root->left == NULL){
            Node* temp = root->right;

            delete root;
            cout <<"Data karyawan berhasil dihapus\n";
            return temp;
        }
        else if(root->right == NULL){
            Node* temp = root->left;
            
            delete root;
            cout <<"Data karyawan berhasil dihapus\n";
            return temp;
        }
        Node* temp = cariKecil(root->right);

        root->nip = temp->nip;
        root->nama = temp->nama;
        root->divisi = temp->divisi;
        root->status = temp->status;

        root->right =
        hapus(root->right, temp->nip);
    }
    return root;
}


void undo(){
    if(top == NULL){
		
		cout <<"Tidak ada aksi untuk di-undo\n";
        return;
    }
    
    int nip = top->nip;
    string aksi = top->aksi;

    Node* karyawan = search(root, nip);
    
    if(karyawan != NULL){
        if(aksi =="pengajuan"){
            karyawan->status = "Tidak Cuti";
            cout <<"Undo :Status cuti karyawan dibatalkan\n";
        }
        else if(aksi == "selesai"){
            karyawan->status = "Cuti";
            cout<<"Undo :Status cuti karyawan dikembalikan menjadi cuti\n";
        }
    }
    pop();
}

/*tampilkan antrian*/
void tampilAntrian(){
	
    if(front == NULL){
        cout <<"\nAntrian kosong\n";
        return;
    }
    Queue* bantu = front;
    cout <<"\n==============================\n";
    cout <<left;
    cout << setw(10)<<"NIP"
         << setw(25) <<"Nama"
         << endl;
    cout <<"==============================\n";

    while(bantu != NULL){
        Node* karyawan = search(root, bantu->nip);

        if(karyawan != NULL) {
            cout <<setw(10) << karyawan->nip
                 << setw(25)<< karyawan->nama
                 <<endl;
        }
        bantu = bantu->next;
    }
    cout <<"==============================\n";
}
int main(){
    int pilihan;

    do{
        cout <<"\n============ MENU ============\n";
        cout << " 1. Tambah Karyawan\n";
        cout << " 2. Tampil Karyawan\n";
        cout << " 3. Pengajuan Cuti\n";
        cout << " 4. Selesai Cuti\n";
        cout << " 5. Hapus Karyawan\n";
        cout << " 6. Undo\n";
        cout << " 7. Tampil Antrian\n";
        cout << " 0. Keluar\n";
        cout << "==============================\n";

        cout <<"Pilih menu :";
        cin >> pilihan;

        if(pilihan == 1){
            int nip;
            string nama;
            string divisi;

            cout <<"\nMasukkan NIP : ";
            cin >> nip;
            cin.ignore();

            cout <<"Masukkan Nama : ";
            getline(cin, nama);

            cout <<"Masukkan Divisi : ";
            getline(cin, divisi);

            insert(root, nip, nama, divisi);
            
        } 
        else if(pilihan == 2) {
            tampilKaryawan();
        }
        else if(pilihan == 3){
            pengajuanCuti();
        }
        else if(pilihan == 4){
            selesaiCuti();
        }
        else if(pilihan == 5){
            int nip;

            cout <<"\nMasukkan NIP : ";
            cin >> nip;

            Node* cek =search(root, nip);

            if(cek == NULL) {
                cout<<"Karyawan tidak ditemukan\n";
            } else {
                root = hapus(root, nip);
            }
        }
        
        else if(pilihan == 6) {
            undo();
        }
        else if(pilihan == 7) {
            tampilAntrian();
        }
        
    } while(pilihan != 0);
    
    return 0;
}
