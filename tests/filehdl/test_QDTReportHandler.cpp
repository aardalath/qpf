#include "test_QDTReportHandler.h"

//#define CheckResultOf(s,r) do {                                         \
//    ev.clear();                                                         \
//    ev.set(std::string( #s ));                                          \
//    EXPECT_EQ(ev.getValue(), r);                                        \
//    } while (0)

namespace TestQDTReportHandler {

TEST_F(TestQDTReportHandler, Test_QDTRptHdl) {
    std::string reportContent(
"{\"EUC_SIM_VIS-34080-1-W-Nominal_20250520T063201.0Z_01.00.fits\":{\"CCD_1-6\":"
"{\"Q-1\":{\"diagnostics\":{\"Electronic_Offset\":{\"name\":\"Electronic_Offset"
"\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"value\":200.025087983911"
"53}},\"Overflow_Pixels\":{\"name\":\"Overflow_Pixels\",\"result\":{\"outcome\""
":\"Nominal\"},\"values\":{\"number\":0}},\"Readout_Noise\":{\"name\":\"Readout"
"_Noise\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"value\":1.46949610"
"41027172}},\"Saturated_pixels\":{\"name\":\"Saturated_pixels\",\"result\":{\"o"
"utcome\":\"Nominal\"},\"values\":{\"number\":0}},\"Saturation_Level\":{\"name\""
":\"Saturation_Level\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"map_"
"ADU\":[[1297,464,202,160536.59375,173045.09375],[1296,464.5,61,159436.09375,17"
"2468.5],[1298,464.5,7,172322.796875,172759.90625],[1605,1661.5,35,156940.59375"
",171814.40625],[1606,1661.5,7,171898.09375,172970.703125]]}},\"Statistics\":{\""
"name\":\"Statistics\",\"result\":{\"messages\":[\"Std.Dev. threshold (150) ex"
"ceeded\"],\"outcome\":\"Warning\"},\"values\":{\"average\":-2332.303377663727,"
"\"median\":-2418,\"std\":1641.5975057454245}},\"Underflow_Pixels\":{\"name\":\""
"Underflow_Pixels\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"number\""
":0}}},\"processing\":{\"BIAS_COR\":{\"value\":1000},\"GAIN_COR\":{\"value\":3"
".1}}},\"Q-2\":{\"diagnostics\":{\"Electronic_Offset\":{\"name\":\"Electronic_O"
"ffset\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"value\":200.0189291"
"1010557}},\"Overflow_Pixels\":{\"name\":\"Overflow_Pixels\",\"result\":{\"outc"
"ome\":\"Nominal\"},\"values\":{\"number\":0}},\"Readout_Noise\":{\"name\":\"Re"
"adout_Noise\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"value\":1.478"
"162114445415}},\"Saturated_pixels\":{\"name\":\"Saturated_pixels\",\"result\":"
"{\"outcome\":\"Nominal\"},\"values\":{\"number\":0}},\"Saturation_Level\":{\"n"
"ame\":\"Saturation_Level\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\""
"map_ADU\":[[1148,27,8,161461.953125,165933.703125]]}},\"Statistics\":{\"name\""
":\"Statistics\",\"result\":{\"messages\":[\"Std.Dev. threshold (150) exceeded\""
"],\"outcome\":\"Warning\"},\"values\":{\"average\":-2347.976286844222,\"media"
"n\":-2418,\"std\":792.1386082283798}},\"Underflow_Pixels\":{\"name\":\"Underfl"
"ow_Pixels\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"number\":0}}},\""
"processing\":{\"BIAS_COR\":{\"value\":1000},\"GAIN_COR\":{\"value\":3.1}}},\""
"Q-3\":{\"diagnostics\":{\"Electronic_Offset\":{\"name\":\"Electronic_Offset\","
"\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"value\":200.02735042735043}"
"},\"Overflow_Pixels\":{\"name\":\"Overflow_Pixels\",\"result\":{\"outcome\":\""
"Nominal\"},\"values\":{\"number\":0}},\"Readout_Noise\":{\"name\":\"Readout_No"
"ise\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"value\":1.48056374807"
"9826}},\"Saturated_pixels\":{\"name\":\"Saturated_pixels\",\"result\":{\"outco"
"me\":\"Nominal\"},\"values\":{\"number\":0}},\"Saturation_Level\":{\"name\":\""
"Saturation_Level\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"map_ADU\""
":[[262,1177.5,59,169914.09375,173082.296875],[261,1177,6,170227.203125,170689"
".09375]]}},\"Statistics\":{\"name\":\"Statistics\",\"result\":{\"messages\":[\""
"Std.Dev. threshold (150) exceeded\"],\"outcome\":\"Warning\"},\"values\":{\"a"
"verage\":-2337.071306827887,\"median\":-2418,\"std\":1113.4418961469928}},\"Un"
"derflow_Pixels\":{\"name\":\"Underflow_Pixels\",\"result\":{\"outcome\":\"Nomi"
"nal\"},\"values\":{\"number\":0}}},\"processing\":{\"BIAS_COR\":{\"value\":100"
"0},\"GAIN_COR\":{\"value\":3.1}}},\"Q-4\":{\"diagnostics\":{\"Electronic_Offse"
"t\":{\"name\":\"Electronic_Offset\",\"result\":{\"outcome\":\"Nominal\"},\"val"
"ues\":{\"value\":200.03288084464555}},\"Overflow_Pixels\":{\"name\":\"Overflow"
"_Pixels\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"number\":0}},\"Re"
"adout_Noise\":{\"name\":\"Readout_Noise\",\"result\":{\"outcome\":\"Nominal\"}"
",\"values\":{\"value\":1.4760630140943263}},\"Saturated_pixels\":{\"name\":\"S"
"aturated_pixels\",\"result\":{\"outcome\":\"Nominal\"},\"values\":{\"number\":"
"0}},\"Saturation_Level\":{\"name\":\"Saturation_Level\",\"result\":{\"outcome\""
":\"Nominal\"},\"values\":{\"map_ADU\":[[603,97,14,166784.65625,173321],[1551,"
"464,54,157662.90625,171767.90625],[1552,464,8,171560.203125,173001.703125],[19"
"75,2011,6,153977,157563.703125]]}},\"Statistics\":{\"name\":\"Statistics\",\"r"
"esult\":{\"messages\":[\"Std.Dev. threshold (150) exceeded\"],\"outcome\":\"Wa"
"rning\"},\"values\":{\"average\":-2344.572935556028,\"median\":-2418,\"std\":1"
"085.328809959921}},\"Underflow_Pixels\":{\"name\":\"Underflow_Pixels\",\"resul"
"t\":{\"outcome\":\"Nominal\"},\"values\":{\"number\":0}}},\"processing\":{\"BI"
"AS_COR\":{\"value\":1000},\"GAIN_COR\":{\"value\":3.1}}},\"diagnostics\":{\"St"
"atistics\":{\"name\":\"Statistics\",\"result\":{\"messages\":[\"Std.Dev. thres"
"hold (150) exceeded\"],\"outcome\":\"Warning\"},\"values\":{\"average\":-2344."
"572935556028,\"median\":-2418,\"std\":1085.328809959921}}}}}}"
                              );
    
    Json::Value v;
    Json::Reader reader;

    reader.parse(reportContent, v);

    QDTReportHandler qrh(std::string("/tmp/qdt.json"));
    qrh.setData(v);
    EXPECT_EQ(qrh.write(), true);

    std::vector<Alert*> issues;
    EXPECT_EQ(qrh.getIssues(issues), true);
}

}           
