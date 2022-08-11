#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int musteriSayisi=0,bireyselMusteriSayisi,ticariMusteriSayisi,i,j,geciciTamSayi,geciciTamSayi2;
int hata,kacinciMusteri,kacinciHesap,gunlukCekilenToplamPara=0, yanlisGirmeHakki = 3;

FILE *dosya1, *dosya2;

char tarihString[25], girilenSifre[30];

time_t simdikiZaman;

typedef struct
{
    int hesapNo;
    int hesapBakiyesi;
}Hesap;


typedef struct
{
    char adSoyad[50];
    int musteriNo;
    int musteriTipi;
    int hesapSayisi;
    Hesap hesapDizisi[2000];
    char dogumYeri[20];
    char dogumTarihi[20];
    char musteriSifresi[30];

}Musteri;


Musteri musteriler[100];
Musteri *musteriPointer = musteriler;


typedef struct
{
    char tarih[30];
    int hesapNo;
    char tutar[80];
    char islem[80];
}Rapor;

Rapor rapordizisi[10000];
Rapor *raporPointer = rapordizisi;


typedef struct
{
    int toplamMusteriSayisi;
    int toplamPara;
    int toplamHesapSayisi;
    int toplamIslemSayisi;
}Banka;

Banka banka;
Banka *bankaPointer = &banka;


void Menu()
{
    int secim=10;

    while (1)
    {
        printf("\n======================= BANKA OTOMASYONU ========================");
        printf("\n\n\t\t\t1) Musteri Ekle\n\t\t\t2) Hesap Actir\n\t\t\t3) Para Yatir\n\t\t\t4) Para Cek\n\t\t\t5) Havale Yap\n\t\t\t6) Hesap Ozetini Iste\n\t\t\t7) Hesabi Kapat\n\t\t\t0) Cikis\n\n");

        printf("Seciminizi girin:");
        scanf(" %d",&secim);

        if (secim == 0)
        {
            printf("\n\nProgram sonlandi! Iyi gunler dileriz...\n");
            break;
        }

        else
            switch(secim)
            {

            case 1:
                system("cls");
                MusteriEkle();
                break;

            case 2:
                system("cls");
                HesapAc();
                break;

            case 3:
                system("cls");
                ParaYatir();
                break;

            case 4:
                system("cls");
                ParaCek();
                break;

            case 5:
                system("cls");
                Havale();
                break;

            case 6:
                system("cls");
                HesapOzeti();
                break;

            case 7:
                system("cls");
                HesapSil();
                break;

            default:
                printf("\nLutfen 0 ile 7 arasinda bir deger girin!");
                Menu();
                break;

            }
    }
}

void MusteriBilgileriYaz()
{
    dosya1 = fopen("bireysel.txt","w");
    dosya2 = fopen("ticari.txt","w");

    for (i=0; i<musteriSayisi; i++)
    {
        if ((musteriPointer+i)->musteriTipi == 1)
        {

            fwrite((musteriPointer+i),sizeof(Musteri),1,dosya1);

        }
        else if ((musteriPointer+i)->musteriTipi == 2)
        {

            fwrite((musteriPointer+i),sizeof(Musteri),1,dosya2);

        }

    }

    fclose(dosya1);
    fclose(dosya2);
}

void BankaBilgileriYaz()
{
    dosya1 = fopen("banka.txt","w");

    fwrite(bankaPointer,sizeof(Banka),1,dosya1);

    fclose(dosya1);
}

void raporBilgileriniYaz()
{
    dosya1 = fopen("rapor.txt","w");

    for (i=0; i <= bankaPointer->toplamIslemSayisi; i++)
    {
        fwrite((raporPointer+i),sizeof(Rapor),1,dosya1);
    }

    fclose(dosya1);

}


void MusteriBilgileriOku()
{
    dosya1 = fopen("bireysel.txt","r");

    while (fread((musteriPointer+musteriSayisi),sizeof(Musteri),1,dosya1))
    {
        musteriSayisi++;
    }


    fclose(dosya1);

    bireyselMusteriSayisi = musteriSayisi;

    dosya2 = fopen("ticari.txt","r");

    while (fread((musteriPointer+musteriSayisi),sizeof(Musteri),1,dosya2))
    {
        musteriSayisi++;
    }

    fclose(dosya2);

    ticariMusteriSayisi = musteriSayisi - bireyselMusteriSayisi;

}

void BankaBilgileriniOku()
{
    dosya1 = fopen("banka.txt","r");

    fread(bankaPointer,sizeof(Banka),1,dosya1);

    fclose(dosya1);
}

void raporBilgileriniOku()
{
    dosya1 = fopen("rapor.txt","r");

    i=0;

    while(fread((raporPointer+i),sizeof(Rapor),1,dosya1))
    {
        i++;
    }

    fclose(dosya1);

}

void BankaBilgileriniDosyayaYaz()
{
    dosya1 = fopen("bankaVerileri.txt","w");

    fprintf(dosya1,"\nBankanin islem ozeti asagidadir....");

    fprintf(dosya1,"\n\nTARIH\t\t\t\t HESAP NO\t\t\t\tISLEM\t\t\t\t\t\t\tTUTAR");
    fprintf(dosya1,"\n-----\t\t\t\t  -----\t\t\t\t\t-----\t\t\t\t\t\t\t-----");

    for (i=0; i< bankaPointer->toplamIslemSayisi; i++)
    {
        strcpy(tarihString,(raporPointer+i)->tarih);
        tarihString[24] = ' ';////son indisi bosluk atamazsam son indiste default olarak \n oluyor ve alt satira kayiyor.

        fprintf(dosya1,"\n%s\t  %d\t\t\t\t\t%s\t\t%s",tarihString,(raporPointer+i)->hesapNo,(raporPointer+i)->islem,(raporPointer+i)->tutar);

    }
    fprintf(dosya1,"\n\nBanka verileri asagidaki gibidir...\n");

    for (i=0; i<musteriSayisi; i++)
    {
        char musteriTipiChar[10];

        if ((musteriPointer+i)->musteriTipi == 1)
            strcpy(musteriTipiChar,"Bireysel");

        else if ((musteriPointer+i)->musteriTipi == 2)
            strcpy(musteriTipiChar,"Ticari");

        fprintf(dosya1,"\nMusteri %d --> Ad Soyad: %s   |   Dogum Tarihi: %s   |   Dogum Yeri: %s   |   Musteri No: %d   |   Musteri Tipi: %s\n\n",i+1,(musteriPointer+i)->adSoyad,(musteriPointer+i)->dogumTarihi,(musteriPointer+i)->dogumYeri,(musteriPointer+i)->musteriNo,musteriTipiChar);

        for (j=0; j<(musteriPointer+i)->hesapSayisi; j++)
        {
            fprintf(dosya1,"\nMusterinin %d. hesabi --> Hesap No: %d   |  Hesap Bakiyesi: %d",j+1,(musteriPointer+i)->hesapDizisi[j].hesapNo,(musteriPointer+i)->hesapDizisi[j].hesapBakiyesi);
        }
        fprintf(dosya1,"\n\n------------------------------------------------------------------------------------------------------------\n");

    }
    fprintf(dosya1,"\nBankadaki toplam musteri sayisi = %d",bankaPointer->toplamMusteriSayisi);
    fprintf(dosya1,"\nBankadaki toplam hesap sayisi = %d",bankaPointer->toplamHesapSayisi);
    fprintf(dosya1,"\nBankadaki toplam para = %d TL\n",bankaPointer->toplamPara);

    fclose(dosya1);
}

void MusteriBilgileriniDosyayaYaz()
{
    dosya1 = fopen("bireyselMusteriVerileri.txt","w");
    dosya2 = fopen("ticariMusteriVerileri.txt","w");

    for (i=0; i<musteriSayisi; i++)
    {
        char musteriTipiChar[10];

        if ((musteriPointer+i)->musteriTipi == 1)
        {
            strcpy(musteriTipiChar,"Bireysel");

            fprintf(dosya1,"\nMusteri %d --> Ad Soyad: %s   |   Dogum Tarihi: %s   |   Dogum Yeri: %s   |   Musteri No: %d   |   Musteri Tipi: %s\n\n",i+1,(musteriPointer+i)->adSoyad,(musteriPointer+i)->dogumTarihi,(musteriPointer+i)->dogumYeri,(musteriPointer+i)->musteriNo,musteriTipiChar);

            for (j=0; j<(musteriPointer+i)->hesapSayisi; j++)
            {
                fprintf(dosya1,"\nMusterinin %d. hesabi --> Hesap No: %d   |  Hesap Bakiyesi: %d",j+1,(musteriPointer+i)->hesapDizisi[j].hesapNo,(musteriPointer+i)->hesapDizisi[j].hesapBakiyesi);
            }
            fprintf(dosya1,"\n\n------------------------------------------------------------------------------------------------------------\n");

        }

        else if ((musteriPointer+i)->musteriTipi == 2)
        {
            strcpy(musteriTipiChar,"Ticari");

            fprintf(dosya2,"\nMusteri %d --> Ad Soyad: %s   |   Dogum Tarihi: %s   |   Dogum Yeri: %s   |   Musteri No: %d   |   Musteri Tipi: %s\n\n",i+1,(musteriPointer+i)->adSoyad,(musteriPointer+i)->dogumTarihi,(musteriPointer+i)->dogumYeri,(musteriPointer+i)->musteriNo,musteriTipiChar);

            for (j=0; j<(musteriPointer+i)->hesapSayisi; j++)
            {
                fprintf(dosya2,"\nMusterinin %d. hesabi --> Hesap No: %d   |  Hesap Bakiyesi: %d",j+1,(musteriPointer+i)->hesapDizisi[j].hesapNo,(musteriPointer+i)->hesapDizisi[j].hesapBakiyesi);
            }
            fprintf(dosya2,"\n\n------------------------------------------------------------------------------------------------------------\n");

        }

    }
    fclose(dosya1);
    fclose(dosya2);
}

void MusteriEkle()
{
    srand(time(NULL));

    printf("\n--------------- MUSTERI EKLEME -----------------");

    printf("\nMusteri ad ve soyadini girin:");
    scanf(" %[^\n]s",(musteriPointer+musteriSayisi)->adSoyad);

    printf("\nMusteri dogum yerini girin:");
    scanf(" %s",(musteriPointer+musteriSayisi)->dogumYeri);

    printf("\nMusteri dogum tarihini girin(10.01.2000 formatinda!):");
    scanf(" %s",(musteriPointer+musteriSayisi)->dogumTarihi);

    printf("\nLutfen bir sifre belirleyin:");
    scanf(" %s",(musteriPointer+musteriSayisi)->musteriSifresi);

    printf("\nMusteri tipini girin(Bireysel->1  Ticari->2):");
    scanf(" %d",&(musteriPointer+musteriSayisi)->musteriTipi);

    while (((musteriPointer+musteriSayisi)->musteriTipi) != 1 && ((musteriPointer+musteriSayisi)->musteriTipi) != 2)
    {
        printf("Lutfen 1 veya 2 girin!");

        printf("\nMusteri tipini girin(Bireysel->1  Ticari->2):");
        scanf(" %d",&(musteriPointer+musteriSayisi)->musteriTipi);
    }

    geciciTamSayi = rand() % 1000 + 1;

    for (i=0;i<musteriSayisi;i++)
    {
        if (geciciTamSayi == (musteriPointer+i)->musteriNo)
        {
            geciciTamSayi = rand() % 1000 + 1;
            i=0;
        }
    }

    (musteriPointer+musteriSayisi)->musteriNo = geciciTamSayi;

    printf("\nSayin %s, kaydiniz basarili! Musteri numaraniz = %d",(musteriPointer+musteriSayisi)->adSoyad,(musteriPointer+musteriSayisi)->musteriNo);

    (musteriPointer+musteriSayisi)->hesapSayisi = 0;

    musteriSayisi++;
    bankaPointer->toplamMusteriSayisi++;

    MusteriBilgileriYaz();
    BankaBilgileriYaz();

    BankaBilgileriniDosyayaYaz();
    MusteriBilgileriniDosyayaYaz();
}

void HesapAc()
{
    hata = 1;

    srand(time(NULL));

    printf("\n----------------------- HESAP ACMA -----------------------");

    while (hata == 1)
    {
        printf("\nHesap acilacak musteri numarasini girin:");
        scanf(" %d",&geciciTamSayi);

        for (i=0; i<musteriSayisi; i++)
        {
            if (geciciTamSayi == (musteriPointer+i)->musteriNo)
            {
                hata = 0;
                kacinciMusteri = i;
                break;
            }

            else if (i == musteriSayisi - 1)
            {
                printf("Boyle bir musteri bulunamadi!");
            }
        }
    }

    yanlisGirmeHakki = 3;

    printf("\nLutfen musteri sifrenizi girin:");
    scanf(" %s",girilenSifre);

    printf("\n");

    while (strcmp(girilenSifre,(musteriPointer+kacinciMusteri)->musteriSifresi) != 0)
    {
        yanlisGirmeHakki--;

        printf("\nYanlis sifre girdiniz! Kalan deneme hakkiniz = %d dir.",yanlisGirmeHakki);

        if (yanlisGirmeHakki == 0)
        {
            printf("\nDeneme hakkiniz bitmistir! Lutfen daha sonra tekrar deneyin.");
            return;
        }

        printf("\nLutfen tekrar deneyin:");
        scanf(" %s",girilenSifre);
    }

    if ((musteriPointer+kacinciMusteri)->hesapSayisi == 10)
    {
        printf("Sayin %s, acabileceginiz maksimum hesap sinirindasiniz(10), yeni hesap acamazsiniz! Yeni hesap acmak icin, eski hesaplarinizdan birini silmelisiniz.",(musteriPointer+kacinciMusteri)->adSoyad);
        return;
    }

    else
    {
    geciciTamSayi2 = rand() % 10000 + 1; //hesapNo atama

    int bulundu = 0;

    for (i=0; i<musteriSayisi; i++)
    {
        if (bulundu == 1)
        {
            i = 0; // i = 1 olacagindan tekrar 0 yapiyoruz.
            bulundu = 0;
        }

        for (j=0; j<(musteriPointer+i)->hesapSayisi; j++)//musterilerin tum hesaplari arasinda kontrol ediyoruz
        {
            if ((musteriPointer+i)->hesapDizisi[j].hesapNo == geciciTamSayi2)
            {
                printf("Hesap no bulundu, tekrar ataniyor..\n");

                geciciTamSayi2 = rand() % 10000 + 1;

                bulundu = 1;

                i = 0;

                break;
            }
        }
    }


    kacinciHesap = (musteriPointer+kacinciMusteri)->hesapSayisi;

    (musteriPointer+kacinciMusteri)->hesapDizisi[kacinciHesap].hesapNo = geciciTamSayi2;

    (musteriPointer+kacinciMusteri)->hesapSayisi++;

    printf("Yeni hesap no: %d, hesap sayiniz: %d, kalan hesap acma hakkiniz: %d\n",(musteriPointer+kacinciMusteri)->hesapDizisi[kacinciHesap].hesapNo,kacinciHesap+1,9-kacinciHesap);

    }

    strcpy((raporPointer+(bankaPointer->toplamIslemSayisi))->islem, "Hesap acildi");

    strcpy((raporPointer+(bankaPointer->toplamIslemSayisi))->tutar, "\t\t\t\t YOK"); //Tablar daha iyi bir gorunumu saglar.

    (raporPointer+(bankaPointer->toplamIslemSayisi))->hesapNo = geciciTamSayi2;

    time(&simdikiZaman);

    strcpy((raporPointer+(bankaPointer->toplamIslemSayisi))->tarih, ctime(&simdikiZaman));

    bankaPointer->toplamHesapSayisi++;
    bankaPointer->toplamIslemSayisi++;


    MusteriBilgileriYaz();
    BankaBilgileriYaz();
    raporBilgileriniYaz();

    BankaBilgileriniDosyayaYaz();
    MusteriBilgileriniDosyayaYaz();

}

void ParaYatir()
{
    printf("\n-------------------------- PARA YATIRMA --------------------------");

    int yatirilanParaMiktari;

    hata = 1;

    while (hata == 1)
    {
        printf("\nMusteri numaranizi girin:");
        scanf(" %d",&geciciTamSayi);

        for (i=0; i<musteriSayisi; i++)
        {
            if (geciciTamSayi == (musteriPointer+i)->musteriNo)
            {
                hata = 0;
                break;
            }

            else if (i == musteriSayisi - 1)
            {
                printf("Boyle bir musteri bulunamadi!");
            }
        }
    }

    kacinciMusteri = i;

    yanlisGirmeHakki = 3;

    printf("\nLutfen musteri sifrenizi girin:");
    scanf(" %s",girilenSifre);

    printf("\n");

    while (strcmp(girilenSifre,(musteriPointer+kacinciMusteri)->musteriSifresi) != 0)
    {
        yanlisGirmeHakki--;

        printf("\nYanlis sifre girdiniz! Kalan deneme hakkiniz = %d dir.",yanlisGirmeHakki);

        if (yanlisGirmeHakki == 0)
        {
            printf("\nDeneme hakkiniz bitmistir! Lutfen daha sonra tekrar deneyin.");
            return;
        }

        printf("\nLutfen tekrar girin:");
        scanf(" %s",girilenSifre);
    }

    printf("\nKacinci hesabiniza para yatirmak istersiniz?");

    if ((musteriPointer+kacinciMusteri)->hesapSayisi != 0)
    {
        for (i=0; i<(musteriPointer+kacinciMusteri)->hesapSayisi; i++)
        {
            printf("\n%d. Hesap(hesap no: %d) = %d TL",i+1,(musteriPointer+kacinciMusteri)->hesapDizisi[i].hesapNo,(musteriPointer+kacinciMusteri)->hesapDizisi[i].hesapBakiyesi);
        }

        printf("\n");
        scanf(" %d",&geciciTamSayi2);

        printf("\n%d. hesabiniza kac TL yatirmak istersiniz?:",geciciTamSayi2);
        scanf(" %d",&yatirilanParaMiktari);

        (musteriPointer+kacinciMusteri)->hesapDizisi[geciciTamSayi2-1].hesapBakiyesi += yatirilanParaMiktari;

        printf("\nPara yatirma islemi basarili! %d. hesabinizin yeni bakiyesi %d TL'dir.",geciciTamSayi2,(musteriPointer+kacinciMusteri)->hesapDizisi[geciciTamSayi2-1].hesapBakiyesi);


        (raporPointer+(bankaPointer->toplamIslemSayisi))->hesapNo = (musteriPointer+kacinciMusteri)->hesapDizisi[geciciTamSayi2-1].hesapNo;

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tutar, "\t\t\t\t+%d TL", yatirilanParaMiktari);

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->islem, "Para yatirma");

        time(&simdikiZaman);
        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tarih, ctime(&simdikiZaman));

        bankaPointer->toplamIslemSayisi++;
        bankaPointer->toplamPara += yatirilanParaMiktari;

        BankaBilgileriYaz();
        raporBilgileriniYaz();

    }

    else
    {
        printf("\nHenuz hesabiniz yoktur, lutfen once hesap acin!");
        return;
    }

    MusteriBilgileriYaz();

    BankaBilgileriniDosyayaYaz();
    MusteriBilgileriniDosyayaYaz();
}

void ParaCek()
{
    printf("\n-------------------------------- PARA CEKME --------------------------------");

    hata = 1;

    int cekilecekParaMiktari, cekilenToplamPara = 0,paraCekilecekHesap,takviyeHesap,takviyePara;
    char secim;

    while (hata == 1)
    {
        printf("\nMusteri numaranizi girin:");
        scanf(" %d",&geciciTamSayi);

        for (i=0; i<musteriSayisi; i++)
        {
            if (geciciTamSayi == (musteriPointer+i)->musteriNo)
            {
                hata = 0;
                break;
            }

            else if (i == musteriSayisi - 1)
            {
                printf("\nBoyle bir musteri bulunamadi!");
            }
        }
    }

    kacinciMusteri = i;

    yanlisGirmeHakki = 3;

    printf("\nLutfen musteri sifrenizi girin:");
    scanf(" %s",girilenSifre);

    printf("\n");

    while (strcmp(girilenSifre,(musteriPointer+kacinciMusteri)->musteriSifresi) != 0)
    {
        yanlisGirmeHakki--;

        printf("\nYanlis sifre girdiniz! Kalan deneme hakkiniz = %d dir.",yanlisGirmeHakki);

        if (yanlisGirmeHakki == 0)
        {
            printf("\nDeneme hakkiniz bitmistir! Lutfen daha sonra tekrar deneyin.");
            return;
        }

        printf("\nLutfen tekrar girin:");
        scanf(" %s",girilenSifre);
    }

    for (i=0; i<(musteriPointer+kacinciMusteri)->hesapSayisi; i++)
    {
        printf("\n%d. Hesap = %d TL",i+1,(musteriPointer+kacinciMusteri)->hesapDizisi[i].hesapBakiyesi);
    }

    printf("\nKacinci hesabinizdan cekmek istersiniz?:");
    scanf(" %d",&paraCekilecekHesap);

    printf("\nKac lira cekmek istiyorsunuz?:");
    scanf(" %d",&cekilecekParaMiktari);

    cekilenToplamPara += cekilecekParaMiktari;
    gunlukCekilenToplamPara += cekilenToplamPara;

    if (((musteriPointer+kacinciMusteri)->musteriTipi == 1) && (gunlukCekilenToplamPara > 750))
    {
        printf("\nGunluk toplam para cekme limitiniz olan 750 TL'yi astiniz!");
        return;
    }

    else if (((musteriPointer+kacinciMusteri)->musteriTipi == 2) && (gunlukCekilenToplamPara > 1500))
    {
        printf("\nGunluk toplam para cekme limitiniz olan 1500 TL'yi astiniz!");
        return;
    }

    while ((musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapBakiyesi < cekilecekParaMiktari)
    {
        printf("\n%d. Hesabinizda yeterli bakiye bulunmamaktadir. Bu hesaptan maksimum %d TL cekebilirsiniz.",paraCekilecekHesap,(musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapBakiyesi);

        printf("\nDiger hesaplarinizdaki bakiyeler:");

        for (i=0; i<(musteriPointer+kacinciMusteri)->hesapSayisi; i++)
        {
            printf("\n%d. Hesap = %d TL",i+1,(musteriPointer+kacinciMusteri)->hesapDizisi[i].hesapBakiyesi);
        }

        printf("\nEk bir hesabinizdan takviye ile cekmek ister misiniz?(Evet icin 1, hayir icin farkli bir deger girin):");
        scanf(" %c",&secim);

        if (secim != '1')
        {
            return;
        }

        printf("\nKacinci hesabinizdan takviye yapmak istersiniz?");
        scanf(" %d",&takviyeHesap);

        if (((musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapBakiyesi + (musteriPointer+kacinciMusteri)->hesapDizisi[takviyeHesap-1].hesapBakiyesi) < cekilecekParaMiktari)
        {
            printf("\nTakviye hesapla birlikte de yeterli bakiyeniz bulunmamaktadir!");
            return;
        }

        takviyePara = cekilecekParaMiktari - (musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapBakiyesi;

        (musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapBakiyesi = 0;

        (musteriPointer+kacinciMusteri)->hesapDizisi[takviyeHesap-1].hesapBakiyesi -= takviyePara;

        printf("\nPara cekme basarili! %d. hesabinizin yeni bakiyesi %d TL'dir.",paraCekilecekHesap,(musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapBakiyesi);

        printf("\nTakviye hesabinizda(%d. hesap) kalan bakiye %d TL'dir.",takviyeHesap,(musteriPointer+kacinciMusteri)->hesapDizisi[takviyeHesap-1].hesapBakiyesi);

        (raporPointer+(bankaPointer->toplamIslemSayisi))->hesapNo = (musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapNo;

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tutar, "\t\t\t\t-%d TL", (musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapBakiyesi);

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->islem, "Para cekme");

        time(&simdikiZaman);
        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tarih, ctime(&simdikiZaman));

        bankaPointer->toplamPara -= cekilecekParaMiktari;
        bankaPointer->toplamIslemSayisi++;

        (raporPointer+(bankaPointer->toplamIslemSayisi))->hesapNo = (musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapNo;

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tutar, "\t\t\t\t-%d TL", takviyePara);

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->islem, "Para cekme");

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tarih, ctime(&simdikiZaman));

        BankaBilgileriYaz();
        MusteriBilgileriYaz();
        raporBilgileriniYaz();

        BankaBilgileriniDosyayaYaz();
        MusteriBilgileriniDosyayaYaz();
        return;
    }
    //-------------------------------------NORMAL DURUM--------------------------------------------------------------------
    (musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapBakiyesi -= cekilecekParaMiktari;

    (raporPointer+(bankaPointer->toplamIslemSayisi))->hesapNo = (musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapNo;

    sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tutar, "\t\t\t\t-%d TL", cekilecekParaMiktari);

    sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->islem, "Para cekme");

    time(&simdikiZaman);
    sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tarih, ctime(&simdikiZaman));


    printf("\nPara cekme basarili! %d. hesabinizin yeni bakiyesi %d TL'dir.",paraCekilecekHesap,(musteriPointer+kacinciMusteri)->hesapDizisi[paraCekilecekHesap-1].hesapBakiyesi);

    bankaPointer->toplamPara -= cekilecekParaMiktari;
    bankaPointer->toplamIslemSayisi++;

    MusteriBilgileriYaz();
    BankaBilgileriYaz();
    raporBilgileriniYaz();

    BankaBilgileriniDosyayaYaz();
    MusteriBilgileriniDosyayaYaz();

}

void Havale()
{
    printf("\n----------------------------- HAVALE -----------------------------");

    int bulundu = 0, gonderenMusteriIndisi, aliciMusteriIndisi, gonderenHesapIndisi, aliciHesapIndisi, paraMiktari, asilParaMiktari, bireyselMaksimumParaMiktari;

    printf("\nHavale yapacaginiz hesap numarasini girin(gonderen hesap):");
    scanf(" %d",&geciciTamSayi);

    for (i=0; i<musteriSayisi; i++)
    {
        for (j=0; j<(musteriPointer+i)->hesapSayisi; j++)
        {
            if ( (musteriPointer+i)->hesapDizisi[j].hesapNo == geciciTamSayi )
                {
                    bulundu = 1;
                    gonderenMusteriIndisi = i;
                    gonderenHesapIndisi = j;
                    break;
                }
        }

        if (bulundu == 1)
        {
            break;
        }

    }

    if (bulundu == 0)
    {
        printf("\nBoyle bir hesap bulunamadi!");
        return;
    }

    yanlisGirmeHakki = 3;

    printf("\nLutfen musteri sifrenizi girin:");
    scanf(" %s",girilenSifre);

    printf("\n");

    while (strcmp(girilenSifre,(musteriPointer+gonderenMusteriIndisi)->musteriSifresi) != 0)
    {
        yanlisGirmeHakki--;

        printf("\nYanlis sifre girdiniz! Kalan deneme hakkiniz = %d dir.",yanlisGirmeHakki);

        if (yanlisGirmeHakki == 0)
        {
            printf("\nDeneme hakkiniz bitmistir! Lutfen daha sonra tekrar deneyin.");
            return;
        }

        printf("\nLutfen tekrar girin:");
        scanf(" %s",girilenSifre);
    }

    printf("\nGonderici hesabin bakiyesi = %d",(musteriPointer+gonderenMusteriIndisi)->hesapDizisi[gonderenHesapIndisi].hesapBakiyesi);

    printf("\nHavale yapilacak hesap numarasini girin(alici hesap):");
    scanf(" %d",&geciciTamSayi2);

    bulundu = 0;

    for (i=0; i<musteriSayisi; i++)
    {
        for (j=0; j<(musteriPointer+i)->hesapSayisi; j++)
        {
            if ( (musteriPointer+i)->hesapDizisi[j].hesapNo == geciciTamSayi2 )
                {
                    bulundu = 1;
                    aliciMusteriIndisi = i;
                    aliciHesapIndisi = j;
                    break;
                }
        }

        if (bulundu == 1)
        {
            break;
        }
    }

    if (bulundu == 0)
    {
        printf("\nBoyle bir hesap bulunamadi!");
        return;
    }

    printf("\n%d no'lu hesabin bakiyesi %d TL'dir. %d no'lu hesaba kac TL havale etmek istersiniz?",geciciTamSayi,(musteriPointer+gonderenMusteriIndisi)->hesapDizisi[gonderenHesapIndisi].hesapBakiyesi,geciciTamSayi2);
    scanf(" %d",&paraMiktari);

    if ((musteriPointer+gonderenMusteriIndisi)->musteriTipi == 1)
    {
        asilParaMiktari = (paraMiktari) * (1.02); //%2 kesinti

        printf("\n%d TL para havale etmek icin hesabinizdan %d TL kesilecektir.",paraMiktari,asilParaMiktari);

        bireyselMaksimumParaMiktari = ((musteriPointer+gonderenMusteriIndisi)->hesapDizisi[gonderenHesapIndisi].hesapBakiyesi) * 0.98;

        if (asilParaMiktari > (musteriPointer+gonderenMusteriIndisi)->hesapDizisi[gonderenHesapIndisi].hesapBakiyesi)
        {
            printf("\n%d no'lu hesapta yeterli bakiye bulunmamaktadir! Maksimum %d TL havale edebilirsiniz.(kesinti oldugundan)",geciciTamSayi,bireyselMaksimumParaMiktari);
            return;
        }

        (musteriPointer+gonderenMusteriIndisi)->hesapDizisi[gonderenHesapIndisi].hesapBakiyesi -= asilParaMiktari;
        (musteriPointer+aliciMusteriIndisi)->hesapDizisi[aliciHesapIndisi].hesapBakiyesi += paraMiktari;

        printf("\nHavale islemi basarili!");

        printf("\n%d no'lu hesabin yeni bakiyesi %d TL",geciciTamSayi,(musteriPointer+gonderenMusteriIndisi)->hesapDizisi[gonderenHesapIndisi].hesapBakiyesi);

        (raporPointer+(bankaPointer->toplamIslemSayisi))->hesapNo = geciciTamSayi;

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tutar, "%d no'lu hesaba -%d TL",geciciTamSayi2, asilParaMiktari);

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->islem, "%s adli kisiye havale yapildi", (musteriPointer+aliciMusteriIndisi)->adSoyad);

        time(&simdikiZaman);
        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tarih, ctime(&simdikiZaman));

        bankaPointer->toplamPara += (paraMiktari) * (1.02); //musteriden kesilen miktar bankanin hesabina yatar
        bankaPointer->toplamIslemSayisi++;

        (raporPointer+(bankaPointer->toplamIslemSayisi))->hesapNo = geciciTamSayi2;

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tutar, "%d no'lu hesaptan +%d TL",geciciTamSayi, paraMiktari);

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->islem, "%s adli kisiden havale geldi", (musteriPointer+gonderenMusteriIndisi)->adSoyad);

        time(&simdikiZaman);
        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tarih, ctime(&simdikiZaman));

        MusteriBilgileriYaz();
        BankaBilgileriYaz();
        raporBilgileriniYaz();

        BankaBilgileriniDosyayaYaz();
        MusteriBilgileriniDosyayaYaz();
    }

    else if ((musteriPointer+gonderenMusteriIndisi)->musteriTipi == 2)
    {
        if (paraMiktari > (musteriPointer+gonderenMusteriIndisi)->hesapDizisi[gonderenHesapIndisi].hesapBakiyesi)
        {
            printf("\n%d no'lu hesapta yeterli bakiye bulunmamaktadir! Maksimum %d TL havale edebilirsiniz.",geciciTamSayi,(musteriPointer+gonderenMusteriIndisi)->hesapDizisi[gonderenHesapIndisi].hesapBakiyesi);
            return;
        }

        (musteriPointer+gonderenMusteriIndisi)->hesapDizisi[gonderenHesapIndisi].hesapBakiyesi -= paraMiktari;
        (musteriPointer+aliciMusteriIndisi)->hesapDizisi[aliciHesapIndisi].hesapBakiyesi += paraMiktari;

        printf("\nHavale islemi basarili!");

        printf("\n%d no'lu hesabin yeni bakiyesi %d TL",geciciTamSayi,(musteriPointer+gonderenMusteriIndisi)->hesapDizisi[gonderenHesapIndisi].hesapBakiyesi);

        (raporPointer+(bankaPointer->toplamIslemSayisi))->hesapNo = geciciTamSayi;

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tutar, "%d no'lu hesaba -%d TL",geciciTamSayi2, paraMiktari);

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->islem, "%s adli kisiye havale yapildi", (musteriPointer+aliciMusteriIndisi)->adSoyad);

        time(&simdikiZaman);
        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tarih, ctime(&simdikiZaman));

        bankaPointer->toplamIslemSayisi++;

        (raporPointer+(bankaPointer->toplamIslemSayisi))->hesapNo = geciciTamSayi2;

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tutar, "%d no'lu hesaptan +%d TL",geciciTamSayi, paraMiktari);

        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->islem, "%s adli kisiden havale geldi", (musteriPointer+gonderenMusteriIndisi)->adSoyad);

        time(&simdikiZaman);
        sprintf((raporPointer+(bankaPointer->toplamIslemSayisi))->tarih, ctime(&simdikiZaman));

        BankaBilgileriYaz();
        MusteriBilgileriYaz();
        raporBilgileriniYaz();

        BankaBilgileriniDosyayaYaz();
        MusteriBilgileriniDosyayaYaz();
    }
}

void HesapSil()
{
    printf("\n----------------------------- HESAP SILME -----------------------------");

    int bulundu = 0, silinecekHesapIndisi, musteriIndisi;
    char secim;

    printf("\nSilinecek hesabin hesap no'sunu girin:");
    scanf(" %d",&geciciTamSayi2);

    for (i=0; i<musteriSayisi; i++)
    {
        for (j=0; j<(musteriPointer+i)->hesapSayisi; j++)
        {
            if (geciciTamSayi2 == (musteriPointer+i)->hesapDizisi[j].hesapNo)
            {
                bulundu = 1;
                geciciTamSayi = (musteriPointer+i)->musteriNo;
                musteriIndisi = i;
                silinecekHesapIndisi = j;
                break;
            }
        }
        if (bulundu == 1)
        {
            break;
        }
    }

    if (bulundu == 0)
    {
        printf("\nBoyle bir musteri bulunamadi!");
        return;
    }

    yanlisGirmeHakki = 3;

    printf("\nLutfen musteri sifrenizi girin:");
    scanf(" %s",girilenSifre);

    printf("\n");

    while (strcmp(girilenSifre,(musteriPointer+musteriIndisi)->musteriSifresi) != 0)
    {
        yanlisGirmeHakki--;

        printf("\nYanlis sifre girdiniz! Kalan deneme hakkiniz = %d dir.",yanlisGirmeHakki);

        if (yanlisGirmeHakki == 0)
        {
            printf("\nDeneme hakkiniz bitmistir! Lutfen daha sonra tekrar deneyin.");
            return;
        }

        printf("\nLutfen tekrar girin:");
        scanf(" %s",girilenSifre);
    }

    if ((musteriPointer+musteriIndisi)->hesapDizisi[silinecekHesapIndisi].hesapBakiyesi != 0)
    {
        printf("\n%d no'lu hesabinizda %d TL bakiye bulunmaktadir, bu hesabi silmek icin hesap bakiyesinin 0 TL olmasi gerekmektedir.",geciciTamSayi2,(musteriPointer+i)->hesapDizisi[silinecekHesapIndisi].hesapBakiyesi);
        return;
    }

    else if ((musteriPointer+musteriIndisi)->hesapDizisi[silinecekHesapIndisi].hesapBakiyesi == 0)
    {
        printf("\nHesabi silmek istediginize emin misiniz? Bu islem geri alinamaz!");

        printf("\nEvet icin 1, iptal etmek icin farkli bir deger girin:");
        scanf(" %c",&secim);

        if (secim != '1')
        {
            printf("\nIslem iptal edildi..");
            return;
        }

        else
        {
            for (j=silinecekHesapIndisi; j < (musteriPointer+musteriIndisi)->hesapSayisi ; j++)
                {
                    (musteriPointer+musteriIndisi)->hesapDizisi[j].hesapNo = (musteriPointer+musteriIndisi)->hesapDizisi[j+1].hesapNo;
                    (musteriPointer+musteriIndisi)->hesapDizisi[j].hesapBakiyesi = (musteriPointer+musteriIndisi)->hesapDizisi[j+1].hesapBakiyesi;
                }

        (musteriPointer+musteriIndisi)->hesapSayisi--;

        }
    }

    bankaPointer->toplamHesapSayisi--;

    BankaBilgileriYaz();
    MusteriBilgileriYaz();

    BankaBilgileriniDosyayaYaz();
    MusteriBilgileriniDosyayaYaz();

    printf("\n%d no'lu hesabiniz basariyla silinmistir..",geciciTamSayi2);

    printf("\nKalan hesaplariniz:");

    for (j=0; j<(musteriPointer+musteriIndisi)->hesapSayisi; j++)
    {
        printf("\nHesap %d --> Hesap no: %d  |  Hesap Bakiyesi: %d",j+1,(musteriPointer+musteriIndisi)->hesapDizisi[j].hesapNo,(musteriPointer+musteriIndisi)->hesapDizisi[j].hesapBakiyesi);
    }

    printf("\n");
}

void HesapOzeti()
{
    printf("\n------------------------------ HESAP OZETI ------------------------------");

    hata = 1;

    while (hata == 1)
    {
        printf("\nMusteri numaranizi girin:");
        scanf(" %d",&geciciTamSayi);

        for (i=0; i<musteriSayisi; i++)
        {
            if (geciciTamSayi == (musteriPointer+i)->musteriNo)
            {
                hata = 0;
                break;
            }

            else if (i == musteriSayisi - 1)
            {
                printf("Boyle bir musteri bulunamadi!");
            }
        }
    }
    kacinciMusteri = i;

    yanlisGirmeHakki = 3;

    printf("\nLutfen musteri sifrenizi girin:");
    scanf(" %s",girilenSifre);

    while (strcmp(girilenSifre,(musteriPointer+kacinciMusteri)->musteriSifresi) != 0)
    {
        yanlisGirmeHakki--;

        printf("\n\nYanlis sifre girdiniz! Kalan deneme hakkiniz = %d dir.",yanlisGirmeHakki);

        if (yanlisGirmeHakki == 0)
        {
            printf("\nDeneme hakkiniz bitmistir! Lutfen daha sonra tekrar deneyin.");
            return;
        }

        printf("\nLutfen tekrar girin:");
        scanf(" %s",girilenSifre);
    }

    for (i=0; i<(musteriPointer+kacinciMusteri)->hesapSayisi; i++)
    {
        printf("\nHesap %d --> Hesap Numarasi = %d | Bakiye = %d TL",i+1,(musteriPointer+kacinciMusteri)->hesapDizisi[i].hesapNo,(musteriPointer+kacinciMusteri)->hesapDizisi[i].hesapBakiyesi);
    }

    printf("\n\nKacinci hesabinizin hesap ozetini istersiniz?:");
    scanf(" %d",&geciciTamSayi2);

    geciciTamSayi = (musteriPointer+kacinciMusteri)->hesapDizisi[geciciTamSayi2-1].hesapNo;

    dosya1 = fopen("dekont.txt","w");

    printf("\n%d no'lu hesabin hesap ozeti asagidadir..",geciciTamSayi);

    printf("\n\nTARIH\t\t\t\tISLEM\t\t\t\t\t\t\tTUTAR");
    printf("\n-----\t\t\t\t-----\t\t\t\t\t\t\t-----");

    fprintf(dosya1,"%d no'lu hesabin hesap ozeti asagidadir..\n",geciciTamSayi);

    fprintf(dosya1,"\nTARIH\t\t\t\tISLEM\t\t\t\t\t\t\tTUTAR");
    fprintf(dosya1,"\n-----\t\t\t\t-----\t\t\t\t\t\t\t-----");

    for (i=0; i< bankaPointer->toplamIslemSayisi; i++)
    {

        if (geciciTamSayi == (raporPointer+i)->hesapNo)
        {
            strcpy(tarihString,(raporPointer+i)->tarih);
            tarihString[24] = ' '; //son indisi bosluk atamazsak tarihin son indisi default olarak \n oluyor ve alt satira kayiyor.

            printf("\n%s\t%s\t\t%s",tarihString,(raporPointer+i)->islem,(raporPointer+i)->tutar);

            fprintf(dosya1,"\n%s\t%s\t\t%s",tarihString,(raporPointer+i)->islem,(raporPointer+i)->tutar);
        }
    }
    printf("\n");

    fclose(dosya1);

}

int main()
{
    srand(time(NULL));

    MusteriBilgileriOku();
    BankaBilgileriniOku();
    raporBilgileriniOku();

    Menu();

    return 0;
}
