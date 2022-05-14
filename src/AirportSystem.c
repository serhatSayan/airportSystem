/*
 ============================================================================
 Name        : AirportSystem.c
 Author      : tayyardurden
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//190202084, Serhat Sayan

#include <stdio.h>
#include <stdlib.h>


struct ucak{
	int oncelik_id;
	int ucak_id;
	int talep_edilen_inis_saati;
	int gecikme_suresi;
	int inis_saati;
	int kalkis_saati;

};

FILE *input;
FILE *output;
struct ucak inisList [24];
struct ucak kalkisList [24];
int saat=1;



//struct ucak inisIzniIste();
void printKalkisKuyruk();
struct ucak readNextUcak();
void nextUcagiIndir();
void uygunYerAraAdd(struct ucak arayan ,int baslangıc);
void outputFileUpdate();

int main(void) {

	input = fopen("input.txt", "r");
	fseek(input, 42, SEEK_SET);






	while(1){
		int secim;
		printf("1. Kalkis kuyrugunu goruntule\n");
		printf("2. Inis izni iste\n");
		printf("Secim: ");
		scanf("%d",&secim);

		switch (secim){
			case 1:
				printKalkisKuyruk();
				break;
			case 2:
				nextUcagiIndir();
				break;
			default:
				printf("Hatali secim yaptiniz!");

		}

	}
	return EXIT_SUCCESS;
}


//iniş ve kalkış kuyruklarındaki ucakları ekrana yazdırır
void printKalkisKuyruk(){


	printf("------------\n");
	for(int i=0; i<24; i++){
		printf("|%02d|%d|%02d.00|%d|\n", inisList[i].ucak_id, inisList[i].oncelik_id, inisList[i].kalkis_saati, inisList[i].gecikme_suresi);
		printf("------------\n");
	}

}

//input dosyasında sırada bulunan uçağı okur
struct ucak readNextUcak(){
	//printf("readNextUcak\n");
	struct ucak okunanUcak;
	okunanUcak.ucak_id = 0;
	okunanUcak.gecikme_suresi = 0;

	int z = fscanf(input, "%d", &okunanUcak.oncelik_id);
	if(z == -1){
		return okunanUcak;
	}
	fscanf(input, "%d", &okunanUcak.ucak_id);
	fscanf(input, "%d", &okunanUcak.talep_edilen_inis_saati);
	okunanUcak.inis_saati = okunanUcak.talep_edilen_inis_saati;

	return okunanUcak;
}

//input dosyasını saat ile uyuşan ucak bulana kadar okur, bulunca onu iniş kuyruğuna yerleştirmeye çalışır
void nextUcagiIndir(){

	for(int i=0; i<28; i++){
		//printf("nextUcagiIndir\n");
		struct ucak okunanUcak;
		okunanUcak = readNextUcak();

		//düzenlenecek (uçak okunamazsa ne yapılması gerek?)(input file sonu)
		if(okunanUcak.ucak_id==0){
			saat++;
			fseek(input, 42, SEEK_SET);
			nextUcagiIndir();
			break;
		}

		else if(okunanUcak.talep_edilen_inis_saati==saat){
				uygunYerAraAdd(okunanUcak, 0);
				break;
		}
	}
	
	outputFileUpdate();

}

//verilen uçak için iniş kuyruğunda uygun yer arar, uygun yer bulunursa yerleştirir
void uygunYerAraAdd(struct ucak arayan ,int baslangic){
	//printf("uygunYerAraAdd\n");
	int curListPointer = baslangic;
	
	//liste bittiğinde veya ucak 3 gecikmeden sonra inemediğinde döngü kırılır
	while(curListPointer<24 && arayan.gecikme_suresi<4){

		//arayan ucak ile karşılaştırılacak yerin boş olup olmadığına bakar
		if(inisList[curListPointer].ucak_id==0){
			inisList[curListPointer] = arayan;
			printf("%d idli ucak icin inis izni verilmistir\n", arayan.ucak_id);
			//printKalkisKuyruk();
			//printf("\n");
			break;
		}//boş olan yerdeki değişkenin değeri NULL mu?

		
		//arayan ucak ile listedeki ucakların gecikmeyle beraber inis saatlerini karsılastırır
		if((arayan.talep_edilen_inis_saati+arayan.gecikme_suresi)>(curListPointer)){
			
			//arayan ucak 3 kez gecikmiş mi?
			if(arayan.gecikme_suresi==3){

				//karşılaştırıldığı ucak 3 kez gecikmiş mi?
				if(inisList[curListPointer].gecikme_suresi==3){

					//ikisi de 3 kez gecikmiş ucaklardan arayan ucak daha öncelikli mi?
					if(arayan.oncelik_id<inisList[curListPointer].oncelik_id){
						printf("%d idli ucaga onceligi sebebiyle %d.00 saati icin inis izni verilmistir\n", arayan.ucak_id, (arayan.talep_edilen_inis_saati+arayan.gecikme_suresi));
						printf("%d idli ucak acil inis yapmasi gerektigi icin %d idli acil inis yapmasi gereken ucak baska havaalanina yonlendirildi\n",arayan.ucak_id, inisList[curListPointer].ucak_id);
						inisList[curListPointer] = arayan;
						//printKalkisKuyruk();
						//printf("\n");
						break;
					}

					//ikisi de 3 kez ertelenmiş ucakların öncelikleri aynı mı?
					else if(arayan.oncelik_id==inisList[curListPointer].oncelik_id){

						//öncelikleri aynı ise arayan ucağın idsi daha düşük mü?
						if(arayan.ucak_id<inisList[curListPointer].ucak_id){
							printf("%d idli ucaga onceligi sebebiyle %d.00 saati icin inis izni verilmistir\n", arayan.ucak_id, (arayan.talep_edilen_inis_saati+arayan.gecikme_suresi));
							printf("%d idli ucak acil inis yapmasi gerektigi icin %d idli acil inis yapmasi gereken ucak baska havaalanina yonlendirildi\n",arayan.ucak_id, inisList[curListPointer].ucak_id);
							inisList[curListPointer] = arayan;
							//printKalkisKuyruk();
							//printf("\n");
							break;
						
					}}

				}

				//eğer aranan ucak 3 kez gecikmiş, karşılaştırıldığı ucak 3 kez gecikmemiş ise onun yerine aranan ucağı ekle ve
				//karşılaştırılan ucak için listede yer ara
				else {
					struct ucak tmpArayan = inisList[curListPointer];
					inisList[curListPointer] = arayan;
					printf("%d idli ucaga onceligi sebebiyle %d.00 saati icin inis izni verilmistir\n", arayan.ucak_id, (arayan.talep_edilen_inis_saati+arayan.gecikme_suresi));
					tmpArayan.gecikme_suresi++;
					uygunYerAraAdd(tmpArayan, curListPointer+1);
					//printKalkisKuyruk();
					//printf("\n");
					break;
				}
				
			}

			//aranan 3 kez gecikmemiş ise

			//karşılaştırıldığı ucak 3 kez gecikmemişse
			else if(inisList[curListPointer].gecikme_suresi<3){

					//aranan ucağın önceliği karsılastırıldıgından küçük mü? küçük ise arananı listeye ekle ve karşılaştırılan
					//için listede yer ara
					if(arayan.oncelik_id<inisList[curListPointer].oncelik_id){
						struct ucak tmpArayan = inisList[curListPointer];
						inisList[curListPointer] = arayan;
						printf("%d idli ucaga onceligi sebebiyle %d.00 saati icin inis izni verilmistir\n", arayan.ucak_id, (arayan.talep_edilen_inis_saati+arayan.gecikme_suresi));
						tmpArayan.gecikme_suresi++;
						uygunYerAraAdd(tmpArayan, curListPointer+1);
						//printKalkisKuyruk();
						//printf("\n");
						break;
					}

					//iki ucagın öncelikleri eşit, aranan ucağın idsi daha küçükse listeye ekle ve karşılaştırılan ucak için listede
					//yer ara
					else if(arayan.oncelik_id==inisList[curListPointer].oncelik_id){
						if(arayan.ucak_id<inisList[curListPointer].ucak_id){
							struct ucak tmpArayan = inisList[curListPointer];
							inisList[curListPointer] = arayan;
							printf("%d idli ucaga onceligi sebebiyle %d.00 saati icin inis izni verilmistir", arayan.ucak_id, (arayan.talep_edilen_inis_saati+arayan.gecikme_suresi));
							tmpArayan.gecikme_suresi++;
							uygunYerAraAdd(tmpArayan, curListPointer+1);
							//printKalkisKuyruk();
							//printf("\n");
							break;
						}
					}
			}
			arayan.gecikme_suresi++;
		}
		curListPointer++;
	}

	if(curListPointer==24 || arayan.gecikme_suresi==4){
		curListPointer--;
	}
	//ucak listeye eklendi mi? veya listeden çıkarıldı mı?
	if(arayan.ucak_id!=inisList[curListPointer].ucak_id){
		if (baslangic == 0)
		{
			printf("%d id'li ucak icin inis izni verilmemistir\n", arayan.ucak_id);
		}
		else {
			printf("%d id'li ucak icin inis izni iptal edilmistir ve baska havaalanina yonlendirilmistir\n", arayan.ucak_id);
		}
		
	}
	//aranan ucak listeye girebildiyse son inis saati ve kalkış saati belirlenir
	else{
		inisList[curListPointer].inis_saati = inisList[curListPointer].talep_edilen_inis_saati + inisList[curListPointer].gecikme_suresi;
		inisList[curListPointer].kalkis_saati = inisList[curListPointer].inis_saati+1;
	}
}

void outputFileUpdate(){

	//yeni bir output dosyası açar ve listedeki verileri dosyaya yazar
	output = fopen("output.txt", "w+");
	size_t arrsize = sizeof(inisList)/sizeof(inisList[0]);
	fprintf(output, "oncelik_id, ucak_id, talep_edilen_inis_saati, inis_saati, gecikme_suresi, kalkis_saati\n");

	for(int i=0; i < arrsize; i++){
		fprintf(output, "%d", inisList[i].oncelik_id);
		fprintf(output, " ");
		fprintf(output, "%d", inisList[i].ucak_id);
		fprintf(output, " ");
		fprintf(output, "%d", inisList[i].talep_edilen_inis_saati);
		fprintf(output, " ");
		fprintf(output, "%d", inisList[i].inis_saati);
		fprintf(output, " ");
		fprintf(output, "%d", inisList[i].gecikme_suresi);
		fprintf(output, " ");
		fprintf(output, "%d", inisList[i].kalkis_saati);
		fprintf(output, "\n");
	}
}
