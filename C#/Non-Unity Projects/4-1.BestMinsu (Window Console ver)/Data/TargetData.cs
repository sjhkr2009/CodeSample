﻿interface AnalysisTargetData {
	public int[] TargetCodes { get; }
}

public class TargetData : AnalysisTargetData {
	public int[] TargetCodes { get; }
	public TargetData(int[] target) {
		TargetCodes = target;
	}
	
	
	// 국내 모든 상장기업 종목 코드 (22.08.28)
	public static int[] AllListedCompanies { get; } = new int[] {
		426550,	430220,	129920,	403550,	148930,	107600,	430700,	126720,	418210,	368600,	365340,	397030,	341170,	424760,
		328130,	354390,	403870,	426670,	112290,	402030,	396270,	425290,	422040,	421800,	412350,	310210,	377460,	424140,
		148780,	382900,	176590,	362320,	357430,	418170,	399720,	389260,	415580,	419270,	389140,	405640,	412930,	413600,
		388050,	413630,	417310,	396300,	366030,	388720,	288980,	408920,	376930,	146320,	371950,	330730,	406760,	365900,
		370090,	251120,	409570,	405350,	211050,	137080,	136410,	276040,	377330,405350, 211050, 137080, 136410, 276040,
		377330, 373220, 179530, 102370, 169670, 404950, 353590, 200350, 400560,
		396770, 397880, 400840, 298870, 403360, 254160, 404990, 199800, 396690, 397500, 402420, 402340, 377480, 361570,
		400760, 222160, 290090, 372800, 311320, 376300, 357880, 389030, 377300, 376180, 348370, 377450, 382800, 114840,
		248020, 261780, 382480, 391710, 393210, 381970, 376290, 159010, 382840, 388220, 391060, 257720, 260970, 377220,
		99430, 393360, 387310, 329180, 395400, 388800, 273640, 203400, 388790, 271940, 377190, 308080, 139990, 89860,
		99390, 315640, 372910, 58970, 367000, 376980, 259960, 195940, 178920, 323410, 386580, 270660, 377030, 365270,
		137310, 352910, 97520, 357580, 318660, 303530, 232680, 380320, 376190, 379390, 187660, 383310, 383800, 276730,
		363250, 383220, 377630, 361670, 380440, 252990, 352480, 357230, 361610, 377400, 294570, 351330, 59270, 373340,
		367480, 333620, 289220, 361390, 347700, 302440, 314930, 367360, 950220, 378850, 356890, 334970, 247660, 372290,
		338220, 373200, 340930, 189330, 322310, 239890, 363260, 262840, 950210, 366330, 277810, 321820, 248070, 163730,
		348030, 352700, 86710, 375500, 236810, 368770, 257990, 322190, 367460, 253610, 365550, 369370, 357550, 314130,
		335810, 347860, 365590, 64850, 353060, 356860, 286000, 352940, 354200, 367340, 317690, 9900, 352770, 318020,
		129890, 265740, 199820, 299030, 244920, 340570, 348150, 330860, 339770, 290690, 348350, 347000, 301300, 122830,
		214610, 304840, 352820, 348210, 53080, 126340, 359090, 323990, 363280, 318410, 355150, 347770, 291650, 294090,
		293490, 347740, 357120, 353490, 332370, 331920, 348840, 950190, 337930, 60850, 32300, 348950, 357780, 164060,
		357250, 350520, 332570, 101360, 353070, 347890, 298540, 225220, 304100, 343510, 266470, 277880, 262260, 334890,
		351340, 950200, 290520, 330350, 326030, 169330, 229000, 322970, 297890, 351320, 298060, 353190, 353810, 223250,
		353200, 349720, 329020, 344860, 343090, 347140, 198080, 237820, 340350, 92190, 204270, 274090, 294140, 294630,
		344050, 65370, 342550, 341160, 344820, 340440, 341310, 311690, 238170, 62970, 327260, 339950, 340120, 288330,
		340360, 336570, 235980, 226330, 322510, 124560, 335870, 317860, 302550, 338100, 337450, 335890, 336040, 250030,
		290510, 336060, 337840, 278650, 308170, 321550, 297090, 103840, 190650, 322000, 306040, 216080, 272210, 214260,
		84850, 333430, 317400, 279600, 332710,
		333050, 322180, 330590, 13890, 331520, 317530, 331660, 272110, 317870, 318010, 336260, 300120, 336370, 36420,
		158430, 234690, 332290, 331380, 93510, 330990, 244460, 317120, 320000, 256150, 149300, 329560, 311390, 195500,
		328380, 286750, 317830, 228670, 186230, 327970, 282880, 317330, 317770, 318000, 148150, 313760, 317850, 241840,
		300080, 234340, 289010, 327610, 251970, 312610, 309900, 323280, 323230, 293780, 308100, 305090, 322780, 321260,
		3670, 253840, 307930, 323350, 319660, 319400, 950180, 317240, 279060, 125210, 307950, 228760, 99750, 100790,
		192650, 192080, 247540, 299660, 316140, 278280, 246960, 104620, 53580, 311060, 298690, 238200, 263050, 290550,
		307180, 310200, 308700, 310870, 309930, 299900, 307750, 298380, 303360, 100590, 110020, 270870, 307870, 302430,
		290120, 128540, 307280, 299910, 900340, 111710, 179290, 117730, 290660, 207490, 246710, 288490, 267850, 263690,
		217330, 27360, 208340, 263020, 227100, 194700, 268600, 290670, 285490, 290650, 306620, 108490, 153710, 288620,
		306200, 293580, 290720, 293480, 110790, 299170, 219750, 10400, 257370, 291230, 140610, 197140, 303030, 290740,
		173130, 89970, 86820, 302920, 290380, 293940, 110990, 275630, 300720, 290270, 91810, 286940, 80720, 204020,
		297570, 226950, 258050, 298040, 298050, 298020, 298000, 175250, 289080, 276240, 299480, 88260, 245620, 299670,
		294870, 284620, 37030, 16790, 258830, 122310, 163430, 287410, 217910, 950170, 253590, 263700, 18250, 64510,
		277070, 226400, 183490, 6620, 154030, 260660, 68270, 42000, 219420, 267790, 255220, 264660, 284740, 285130,
		269620, 285770, 187870, 281310, 236030, 281740, 282330, 92780, 272450, 260930, 241770, 234300, 281820, 66360,
		7680, 138580, 282690, 272550, 253450, 148140, 950160, 278990, 55490, 280360, 180060, 265560, 234100, 263810,
		255440, 277410, 263920, 243840, 179900, 171090, 259630, 174900, 263540, 263750, 270020, 256940, 260870, 263600,
		267060, 258610, 270520, 140670, 900310, 273060, 118990, 263800, 263860, 263720, 91990, 181340, 162120, 238490,
		271980, 251630, 227610, 272290, 35720, 271560, 3380, 261200, 233250, 267320, 271850, 270210, 266170, 250000,
		267980, 225190, 264850, 161580, 268280, 267290, 251270, 267270, 267260, 267250, 256630, 267810, 166090, 258790,
		251370, 225860, 266870, 264900, 265520, 263770, 63760, 264450, 178320, 183300, 246720, 145720, 217480, 2800,
		83500, 87260, 241820, 111110, 140070, 206650,
		227420, 256840, 242350, 241520, 246690, 258250, 147760, 199290, 196300, 203450, 215600, 254120, 143210, 241790,
		220100, 144960, 156100, 258540, 232830, 176440, 220180, 241560, 148780, 207940, 237880, 900300, 238120, 252500,
		243870, 216050, 239610, 241710, 234080, 900290, 251280, 900280, 189300, 72990, 950140, 238090, 50960, 201490,
		241590, 250930, 229640, 204210, 241690, 250060, 242040, 234920, 249420, 900270, 248170, 230360, 191600, 16740,
		208860, 73560, 212310, 224020, 123010, 66830, 26960, 174880, 71460, 123890, 34830, 142760, 237750, 900260,
		246250, 245450, 195870, 237690, 144510, 148250, 208850, 239340, 243070, 244880, 112190, 70960, 101530, 240810,
		240340, 238500, 228850, 225330, 228340, 4440, 222110, 211270, 230980, 115180, 65660, 236200, 194370, 142210,
		900250, 236340, 67370, 206950, 232530, 109610, 233990, 224060, 226320, 135160, 215570, 232140, 191410, 223310,
		145020, 222980, 206560, 13310, 47920, 217730, 234070, 133750, 225530, 140860, 230240, 122640, 222040, 58110,
		44990, 225850, 217820, 75580, 221840, 180400, 127160, 221980, 212560, 227950, 214370, 185490, 190510, 56090,
		214330, 89590, 226340, 217950, 115960, 92590, 229500, 182400, 227840, 226360, 92870, 224810, 200580, 214870,
		225430, 226440, 225590, 79550, 225570, 217190, 219130, 39570, 2690, 175140, 222080, 220630, 224110, 196700,
		95570, 131760, 222810, 222800, 189980, 222420, 94360, 127710, 224760, 214430, 67730, 222670, 220260, 214450,
		219550, 87010, 214320, 214420, 86220, 223220, 85620, 218410, 94170, 214180, 178780, 214390, 221800, 216400,
		160600, 217600, 166480, 214310, 210980, 103660, 177350, 217270, 218150, 160980, 217500, 87600, 217620, 187420,
		217880, 76340, 215380, 215480, 215790, 215360, 215200, 217320, 216280, 215090, 215100, 214680, 195990, 215000,
		214150, 215050, 138250, 214270, 145210, 189690, 213420, 213500, 210540, 60480, 176750, 206640, 204630, 208640,
		193250, 200470, 200670, 208710, 189860, 67390, 80580, 208350, 160550, 124500, 208890, 28260, 142280, 187220,
		208370, 208140, 149980, 84650, 210120, 200710, 200780, 207760, 196170, 206400, 140520, 150440, 173940, 205470,
		112610, 200230, 204840, 204620, 196490, 18260, 149010, 205500, 205100, 191420, 192440, 182690, 143540, 121060,
		203650, 41920, 194480, 204320, 203690, 192410, 198440, 202960, 200880, 187270, 192400, 177830, 90410, 4650,
		192390, 59120, 200130, 133820, 105550, 136660,
		199150, 196450, 71850, 27410, 187790, 192250, 192820, 138080, 53300, 84440, 154040, 90850, 140660, 189350,
		184230, 178600, 140290, 49080, 150840, 107640, 85810, 185750, 170030, 67570, 134580, 6880, 182360, 171120,
		171010, 161570, 183410, 138360, 183190, 76610, 168330, 64350, 185190, 131970, 150900, 170920, 119850, 180640,
		92040, 181710, 130500, 170790, 151860, 175330, 89600, 114920, 179720, 86460, 116100, 950130, 155660, 141080,
		168490, 170900, 158310, 97800, 104540, 159580, 114810, 153490, 99190, 141020, 159910, 950110, 46970, 113810,
		13870, 155650, 37560, 149950, 163560, 161890, 161390, 121850, 161000, 151910, 68400, 153460, 106520, 137400,
		155900, 14710, 141000, 153360, 72950, 152550, 79980, 143240, 126870, 147830, 91590, 145270, 140410, 104830,
		127120, 7070, 130580, 131090, 100660, 129260, 115480, 145990, 139670, 121800, 138070, 123570, 122870, 136510,
		89530, 89030, 101240, 7820, 115530, 39130, 90360, 131220, 131390, 143160, 78520, 137950, 40910, 123330, 139050,
		23000, 89980, 31430, 140910, 138610, 109080, 108380, 47810, 71840, 19770, 69640, 138490, 137940, 131100, 139480,
		139130, 53210, 138690, 19440, 122450, 121440, 138040, 136540, 136480, 23350, 134060, 104480, 138930, 20150,
		8470, 11210, 136490, 134380, 121600, 93320, 130740, 131180, 61970, 126880, 33560, 48530, 134790, 119860, 131370,
		64290, 131290, 82740, 131400, 117580, 126560, 120240, 58860, 130660, 128660, 131030, 96690, 900140, 33830,
		33170, 41460, 105740, 126600, 123420, 111870, 123860, 58400, 30790, 69140, 119830, 101930, 78650, 68940, 123840,
		81660, 117670, 119500, 58850, 68240, 57050, 96530, 122990, 126700, 126640, 123410, 2150, 123040, 67920, 128820,
		122900, 128940, 119610, 106190, 105840, 114120, 33920, 121890, 123750, 89850, 46120, 102460, 70300, 108320,
		79970, 123690, 123700, 71200, 100030, 122690, 60980, 32830, 31440, 900110, 900100, 115500, 900120, 115450,
		122350, 88350, 115610, 120030, 115310, 106080, 101330, 114570, 120110, 119650, 71320, 115390, 115570, 115440,
		50860, 12160, 114630, 80530, 118000, 900070, 99520, 114450, 104460, 96640, 112040, 52690, 108860, 88290, 114190,
		114090, 115160, 109960, 34730, 99410, 101000, 109820, 32560, 82920, 80, 42520, 82640, 104200, 52860, 47400,
		105330, 93240, 109740, 95700, 108230, 100130, 103230, 39490, 111770, 63080, 109860, 53690, 28100,
		102120, 101730, 102710, 99440, 99220, 101680, 71970, 10240, 101170, 81150, 108670, 98660, 71670, 101490, 100120,
		102940, 101400, 105630, 109070, 106240, 107590, 86890, 104040, 78070, 86900, 58630, 59100, 35420, 67630, 92130,
		26940, 105560, 100700, 104700, 98120, 83470, 100090, 103590, 103140, 11070, 101390, 76080, 101670, 94840, 17180,
		63170, 47560, 100220, 96240, 102260, 101160, 99320, 102280, 64480, 98460, 67000, 18620, 95610, 53280, 96870,
		32640, 59210, 97870, 101140, 20560, 100840, 100250, 91440, 96040, 85310, 97780, 96630, 68330, 93920, 67010,
		96350, 51600, 69920, 96610, 92070, 94820, 86670, 93230, 92440, 95190, 44450, 94280, 92300, 33270, 93640, 92200,
		95910, 81580, 73110, 94970, 80520, 95340, 85910, 95500, 27580, 97950, 86040, 28670, 90740, 57540, 94940, 95270,
		97230, 93190, 96760, 91340, 96770, 72770, 94480, 86520, 65150, 78340, 95660, 29780, 92600, 69410, 96300, 1780,
		93380, 95720, 86450, 64820, 91580, 92460, 73540, 91120, 94850, 94860, 94800, 78020, 93520, 48260, 91970, 92730,
		90460, 39200, 89010, 90470, 88130, 90370, 93370, 84110, 89150, 91700, 86390, 90710, 93050, 60540, 66310, 67280,
		44380, 44820, 89890, 34590, 80470, 89140, 89790, 90150, 88390, 9770, 92220, 92230, 89230, 43150, 86960, 89470,
		38060, 91090, 83660, 62860, 63570, 86980, 68050, 85370, 39290, 90350, 90430, 78860, 88910, 32350, 54950, 90080,
		49800, 84730, 35510, 86060, 22220, 88790, 88800, 88980, 23530, 68760, 83450, 19990, 79370, 46110, 85670, 84370,
		83790, 64550, 84990, 83640, 85660, 86280, 78140, 67900, 83550, 84870, 84180, 86790, 50540, 1560, 80420, 83650,
		79190, 79430, 82800, 43910, 79000, 84010, 41020, 78590, 38070, 75180, 83420, 79940, 77360, 83930, 67310, 83310,
		82660, 80010, 84690, 58220, 71950, 78160, 82850, 80160, 84680, 84670, 42700, 78890, 50890, 4890, 82210, 82270,
		75130, 10960, 73240, 73490, 79950, 79960, 80220, 79170, 79810, 80000, 52220, 79650, 52900, 66910, 49950, 52460,
		79160, 81000, 72870, 73010, 46440, 78600, 54450, 78150, 67990, 14830, 51360, 72520, 41650, 78130, 68790, 23810,
		78940, 48870, 65350, 78930, 77500, 58730, 39340, 34220, 64260, 78000, 11080, 50090, 29960, 57030, 65770, 74430,
		66700,
		66900, 78350, 77970, 75970, 69540, 73190, 54090, 41910, 34310, 19680, 39670, 67770, 70590, 73570, 73640, 65680,
		57880, 24110, 74610, 60570, 66410, 67160, 74600, 71280, 69510, 67170, 54780, 41440, 25750, 72020, 65510, 72470,
		68930, 69110, 7660, 35250, 69330, 64760, 37270, 52710, 72710, 65950, 47310, 71050, 56000, 41520, 65560, 72130,
		34120, 64240, 66790, 69080, 36570, 71090, 67080, 66590, 67290, 66130, 46140, 65450, 65130, 69730, 65440, 6890,
		49630, 21650, 51370, 65650, 65570, 66980, 16100, 11500, 42040, 66970, 66670, 66430, 47820, 64090, 49960, 53290,
		64520, 69960, 35600, 65170, 69460, 34230, 69620, 41590, 27740, 36530, 69260, 42600, 33530, 65500, 60900, 66620,
		52770, 65420, 64800, 58430, 39980, 48410, 68290, 67830, 58610, 45340, 30610, 65710, 65690, 9240, 66110, 43710,
		60560, 36010, 60280, 60230, 46940, 57680, 34300, 60150, 65530, 63440, 17370, 51160, 65060, 49180, 60720, 61040,
		50760, 54210, 50120, 54410, 60310, 66570, 61250, 53980, 24720, 54050, 39420, 64960, 39440, 49550, 54670, 60590,
		49070, 60370, 51380, 53660, 37710, 53270, 34950, 60260, 60380, 46310, 58530, 49720, 52670, 46890, 13990, 58450,
		59090, 50320, 54930, 35200, 54040, 48470, 60300, 42500, 58420, 40350, 54300, 47080, 20760, 60250, 56730, 53260,
		60240, 33180, 41930, 54220, 14940, 43590, 58470, 45300, 56190, 56700, 54940, 43370, 63160, 54620, 56360, 43260,
		38340, 51390, 54540, 53350, 54630, 56080, 44490, 53950, 53700, 54920, 41960, 53160, 52400, 54180, 49470, 53050,
		43090, 53110, 54800, 47770, 53800, 55550, 40300, 58820, 35900, 53590, 21320, 53060, 53610, 52790, 53450, 53030,
		52420, 21240, 53620, 49520, 52300, 48910, 51980, 58650, 51500, 49120, 43360, 52330, 52600, 52020, 43220, 33790,
		51780, 52190, 51490, 38870, 45660, 45390, 50110, 52260, 36540, 51900, 51910, 47040, 47050, 42660, 42670, 38500,
		51630, 48550, 49830, 44060, 24850, 48770, 49430, 44480, 46390, 14620, 48430, 17480, 49480, 36670, 42510, 39830,
		36190, 46070, 41830, 14570, 45520, 22100, 49770, 44180, 44960, 32080, 34020, 10470, 44340, 42940, 10040, 48830,
		250, 44780, 45510, 45100, 40160, 43340, 43650, 37950, 39030, 38540, 18680, 39860,
		46210, 45060, 43100, 42370, 45970, 40420, 7370, 41140, 43610, 36890, 39010, 9300, 43200, 42110, 39740, 39840,
		42420, 39560, 38460, 32820, 1540, 38530, 39310, 41190, 36640, 31330, 30190, 38620, 39020, 38950, 36800, 40610,
		38880, 38680, 41510, 10280, 37760, 36480, 38110, 32500, 39610, 39240, 38290, 37440, 38390, 36830, 36810, 36690,
		37370, 36620, 37350, 38010, 16450, 1810, 36630, 36710, 37070, 16250, 36560, 35890, 36930, 36030, 16710, 32190,
		5290, 3100, 26040, 36180, 36170, 37230, 37460, 37400, 3220, 36460, 33640, 37330, 36090, 36200, 24800, 27050,
		36120, 35290, 27040, 27830, 36000, 7120, 35760, 35810, 35460, 850, 36580, 33780, 9540, 24090, 35620, 35610,
		20710, 12790, 34940, 6840, 6650, 35150, 35000, 33250, 33230, 35080, 5160, 5990, 34810, 33240, 30200, 19490,
		30000, 25770, 33540, 33500, 26890, 29460, 30350, 33160, 33310, 33320, 9520, 13810, 33340, 24890, 33200, 33290,
		15710, 17940, 18670, 23590, 14440, 33100, 33050, 33130, 32980, 19210, 26150, 31310, 11790, 32960, 32680, 32940,
		8290, 10600, 25890, 32750, 32850, 32860, 32790, 32800, 24900, 23960, 23800, 32620, 32580, 32540, 28080, 32280,
		12320, 12340, 25530, 31510, 31820, 31860, 31980, 660, 23150, 5320, 23450, 28050, 29530, 15360, 16800, 11560,
		24740, 24070, 30720, 25560, 30210, 30960, 12630, 17900, 25540, 30530, 30520, 21880, 21040, 29480, 3160, 11930,
		19180, 18880, 28300, 27710, 21820, 27970, 6740, 910, 20120, 20000, 17800, 16580, 23600, 18310, 3800, 14190,
		13580, 21050, 12750, 15890, 12700, 18000, 25980, 26910, 17250, 16590, 11690, 25860, 18500, 11300, 15750, 20180,
		6730, 11200, 12610, 17810, 17390, 25820, 25950, 25900, 11090, 7460, 25620, 17960, 25870, 25880, 2680, 25550,
		25440, 25320, 24940, 24950, 14100, 24840, 24910, 4780, 13120, 24830, 24810, 24880, 25000, 10580, 7980, 24120,
		17000, 24060, 23910, 1840, 23900, 17650, 10170, 23790, 23770, 23760, 5860, 9440, 8420, 16880, 12860, 23460,
		6050, 23410, 23440, 6910, 18290, 23160, 7310, 8500, 14790, 7530, 10120, 1000, 12620, 11370, 9190, 760, 14820,
		18700, 14470, 10140, 9680, 10690, 7720, 13720, 6580, 11320, 18120, 9620,
		5670, 7330, 6140, 19010, 9780, 390, 9450, 20400, 14200, 5710, 16600, 4590, 16670, 440, 12030, 5090, 14970, 5750,
		11040, 8370, 7390, 2290, 19540, 7770, 17890, 3310, 19660, 5880, 8560, 17510, 5870, 17550, 21080, 11170, 8830,
		1430, 8770, 14580, 13520, 4080, 6060, 14530, 2140, 2230, 16090, 1620, 17040, 11390, 11810, 10130, 9730, 12600,
		4720, 9200, 1500, 8870, 8110, 5820, 8490, 14910, 3580, 15860, 14160, 12280, 19170, 2210, 16920, 12170, 9420,
		13030, 11330, 12800, 1140, 13360, 13700, 2760, 7610, 4920, 9290, 11420, 4270, 1940, 9410, 7280, 9180, 2170,
		17670, 10640, 3650, 4380, 2840, 7860, 19550, 13000, 12330, 15540, 1390, 9320, 1450, 5440, 15760, 7690, 6280,
		9160, 8600, 15230, 7160, 14130, 8040, 9140, 9580, 14990, 10420, 13570, 3610, 15590, 14680, 10820, 19570, 19590,
		6040, 8060, 3680, 6920, 15020, 11280, 2310, 10660, 11150, 1770, 18470, 5850, 9970, 12510, 3850, 7630, 8250,
		12690, 6370, 6980, 2920, 2220, 2450, 2460, 7340, 890, 1290, 3010, 14280, 9810, 8730, 9270, 4140, 2960, 3000,
		5810, 1270, 12200, 4710, 16610, 8930, 5500, 5490, 2410, 5690, 1230, 6340, 5950, 16360, 2810, 1750, 11780, 3470,
		2620, 15260, 9460, 5720, 6660, 4490, 11000, 9310, 10100, 5070, 4830, 1720, 1200, 3460, 11700, 5420, 500, 10950,
		12450, 11230, 4020, 4770, 4560, 6090, 3530, 9470, 1510, 10770, 7110, 16380, 10780, 2360, 3830, 7810, 4170,
		10060, 520, 6490, 3120, 720, 2710, 3060, 7700, 8700, 10620, 6360, 6110, 6200, 6400, 9150, 5960, 9070, 3070,
		5180, 5250, 480, 4310, 3920, 8350, 3350, 1880, 11760, 6260, 1740, 2100, 4870, 7590, 4430, 2780, 4090, 650, 430,
		1080, 4700, 4360, 8970, 2880, 1260, 5680, 5800, 2630, 1550, 5430, 4910, 4690, 3080, 2600, 1570, 3960, 2070,
		3570, 4960, 4370, 3720, 4980, 4970, 2870, 1630, 5740, 2390, 1380, 6120, 7540, 3280, 1250, 2990, 1470, 1820,
		1020, 1520, 880, 1060, 670, 1340, 180, 5110, 4450, 40, 2240, 4100, 2820, 4840, 2350, 4060, 4000, 20, 210, 1120,
		2720, 3240, 5030, 6390, 590, 5390, 990, 6570, 220, 860, 3540, 5940, 6800, 2700, 1360, 810, 3230, 370, 8260, 230,
		490, 1800, 4540, 1420, 2020, 5930, 300, 1460, 3620, 3410, 5610, 540, 10050, 7570, 2320, 2200, 5380, 5360, 9830,
		2030, 6380, 4990, 1070, 4410, 3560, 680, 1210, 140, 320, 270, 4800, 1040, 150, 5010, 5830, 2900, 3090, 3780,
		3520, 2380, 5300, 3200, 2270, 2790, 6220, 4150, 7210, 400, 1130, 1680, 3550, 640, 3690, 970, 3300, 4250, 3030,
		1440, 1790, 70, 2420, 240, 950, 3490, 1530, 100, 120, 60, 50, 700, 3480
	};
}